#include "catch2/catch.hpp"

#include <chrono>
#include <vector>
#include "CompletionPort.h"
#include "CompletionStatus.h"
#include "Handle.h"
#include "Overlapped.h"

TEST_CASE("CompletionPort") {
    using namespace laio::iocp;

    // CompletionPort has traits `is_send` and `is_sync`
    CHECK(laio::trait::is_send<CompletionPort>);
    CHECK(laio::trait::is_sync<CompletionPort>);

    // CompletionPort also has the traits `as_raw_handle`, `from_raw_handle` and
    // `into_raw_handle`
    CHECK(laio::trait::as_raw_handle<CompletionPort>);
    CHECK(laio::trait::from_raw_handle<CompletionPort>);
    CHECK(laio::trait::into_raw_handle<CompletionPort>);

    // Associating a new handle or socket to the completion port the tokens must
    // have the same size in memory as `ULONG_PTR`
    CHECK(sizeof std::size_t == sizeof ULONG_PTR);

    // The port attempts to dequeue a completion status until it times out
    CompletionPort port = std::get<CompletionPort>(CompletionPort::create(1));
    const wse::win_error timeout = std::get<1>(port.get(std::chrono::milliseconds(1)));
    CHECK(timeout == wse::win_errc::wait_timeout);
}

TEST_CASE("CompletionStatus") {
    using namespace laio::iocp;

    // CompletionStatus has traits `is_send` and `is_sync`
    CHECK(laio::trait::is_send<CompletionStatus>);
    CHECK(laio::trait::is_sync<CompletionStatus>);

    // `get` dequeues a CompletionStatus from the associated port
    CompletionPort port1 = std::get<CompletionPort>(CompletionPort::create(1));
    Overlapped async1 = Overlapped::zero();
    port1.post(CompletionStatus::create(1, 2, &async1));
    CompletionStatus message = std::get<CompletionStatus>(port1.get(std::nullopt));
    CHECK(message.bytes_transferred() == 1);
    CHECK(message.token() == 2);
    CHECK(message.overlapped() == async1.raw());

    // So does `get_many`, which must be provided with an array of zeroed
    // CompletionStatuses
    CompletionPort port2 = std::get<CompletionPort>(CompletionPort::create(1));
    Overlapped async2 = Overlapped::zero();
    Overlapped async3 = Overlapped::zero();
    port2.post(CompletionStatus::create(1, 2, &async2));
    port2.post(CompletionStatus::create(4, 5, &async3));
    std::vector<CompletionStatus> messageQueue(4, CompletionStatus::zero());

    // View into the message queue
    gsl::span<CompletionStatus> dequeued = std::get<0>(port2.get_many(messageQueue, std::nullopt));
    CHECK(dequeued.size() == 2);
    auto dequeuedIndex = dequeued.begin();

    // First dequeued message
    CHECK(dequeuedIndex->bytes_transferred() == 1);
    CHECK(dequeuedIndex->token() == 2);
    CHECK(dequeuedIndex->overlapped() == async2.raw());

    // Second dequeued message
    std::advance(dequeuedIndex, 1);
    CHECK(dequeuedIndex->bytes_transferred() == 4);
    CHECK(dequeuedIndex->token() == 5);
    CHECK(dequeuedIndex->overlapped() == async3.raw());

    // No more messages dequeued
    std::advance(dequeuedIndex, 1);
    CHECK(dequeuedIndex == dequeued.end());

    // Remaining messages in the target array are still zeroed
    CHECK(messageQueue[2].bytes_transferred() == 0);
    CHECK(messageQueue[2].token() == 0);
    CHECK(messageQueue[2].overlapped() == nullptr);
}