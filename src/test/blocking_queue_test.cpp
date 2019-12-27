#include <thread>
#include <memory>

#include <gtest/gtest.h>

#include "blocking_queue.h"

namespace core {
    namespace {
        TEST(BlockingQueueTest, testPushPop) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue;
            blocking_queue.Push(std::make_unique<int>(42));
            ASSERT_EQ(1, blocking_queue.Size());
            blocking_queue.Push(std::make_unique<int>(24));
            ASSERT_EQ(2, blocking_queue.Size());
            EXPECT_EQ(42, *blocking_queue.Pop());
            ASSERT_EQ(1, blocking_queue.Size());
            EXPECT_EQ(24, *blocking_queue.Pop());
            ASSERT_EQ(0, blocking_queue.Size());
        }

        TEST(BlockingQueueTest, testPushPopSharedPtr) {
            BlockingQueue<std::shared_ptr<int>> blocking_queue;
            blocking_queue.Push(std::make_shared<int>(42));
            blocking_queue.Push(std::make_shared<int>(24));
            EXPECT_EQ(42, *blocking_queue.Pop());
            EXPECT_EQ(24, *blocking_queue.Pop());
        }

        TEST(BlockingQueueTest, testPopWithTimeout) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue;
            EXPECT_EQ(nullptr, blocking_queue.PopWithTimeout(std::chrono::milliseconds(150)));
        }

        TEST(BlockingQueueTest, testPushWithTimeout) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue(1);
            EXPECT_EQ(true, blocking_queue.PushWithTimeout(std::make_unique<int>(42), std::chrono::milliseconds(150)));
            EXPECT_EQ(false, blocking_queue.PushWithTimeout(std::make_unique<int>(25), std::chrono::milliseconds(150)));
            EXPECT_EQ(42, *blocking_queue.Pop());
            ASSERT_EQ(0, blocking_queue.Size());
        }

        TEST(BlockingQueueTest, testPushWithTimeoutWithInfiniteQueue) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue;
            EXPECT_EQ(true, blocking_queue.PushWithTimeout(std::make_unique<int>(42), std::chrono::milliseconds(150)));
            EXPECT_EQ(true, blocking_queue.PushWithTimeout(std::make_unique<int>(25), std::chrono::milliseconds(150)));
            EXPECT_EQ(42, *blocking_queue.Pop());
            EXPECT_EQ(25, *blocking_queue.Pop());
            ASSERT_EQ(0, blocking_queue.Size());
        }


        TEST(BlockingQueueTest, testBlockingPop) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue;
            ASSERT_EQ(0, blocking_queue.Size());

            int pop = 0;

            std::thread thread([&blocking_queue, &pop] { pop = *blocking_queue.Pop(); });

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            blocking_queue.Push(std::make_unique<int>(42));
            thread.join();
            ASSERT_EQ(0, blocking_queue.Size());
            EXPECT_EQ(42, pop);
        }

        TEST(BlockingQueueTest, testBlockingPopWithTimeout) {
            BlockingQueue<std::unique_ptr<int>> blocking_queue;
            ASSERT_EQ(0, blocking_queue.Size());

            int pop = 0;

            std::thread thread([&blocking_queue, &pop] {
                pop = *blocking_queue.PopWithTimeout(std::chrono::milliseconds(2500));
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            blocking_queue.Push(std::make_unique<int>(42));
            thread.join();
            ASSERT_EQ(0, blocking_queue.Size());
            EXPECT_EQ(42, pop);
        }

    }
}
