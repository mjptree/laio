#include "catch2/catch.hpp"

#include <chrono>
#include <iostream>
#include "CompletionPort.h"
#include "CompletionStatus.h"
#include "Handle.h"
#include "Overlapped.h"

TEST_CASE("CompletionPort") {
    using namespace laio::iocp;

    // CompletionPort has traits `is_send` and `is_sync`
    CHECK(laio::trait::is_send<CompletionPort>);
    CHECK(laio::trait::is_sync<CompletionPort>);

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


    // So does `get_many`, which must be provided with an array of zeroed
    // CompletionStatuses
    CompletionPort port = std::get<CompletionPort>(CompletionPort::create(1));
    Overlapped async1 = Overlapped::zero();
    Overlapped async2 = Overlapped::zero();
    port.post(CompletionStatus::create(1, 2, &async1));
    port.post(CompletionStatus::create(4, 5, &async2));
    std::vector<CompletionStatus> messageQueue(4, CompletionStatus::zero());
    {
        // View into the message queue
        gsl::span<CompletionStatus> dequeued = std::get<0>(port.get_many(messageQueue, std::nullopt));
        CHECK(dequeued.size() == 2);

        // First dequeued message
        CHECK(dequeued[0].bytes_transferred() == 1);
        CHECK(dequeued[0].token() == 2);
        /* CHECK(dequeued[0].overlapped() == async1); */

        // Second dequeued message
        CHECK(dequeued[1].bytes_transferred() == 4);
        CHECK(dequeued[1].token() == 5);
        /* CHECK(dequeued[1].overlapped() == async2); */
    }
    // Remaining message
    CHECK(messageQueue[2].bytes_transferred() == 0);
    CHECK(messageQueue[2].token() == 0);
    CHECK(messageQueue[2].overlapped() == nullptr);
}