// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_BIGTABLE_INTERNAL_ASYNC_RETRY_OP_H_
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_BIGTABLE_INTERNAL_ASYNC_RETRY_OP_H_

#include "google/cloud/bigtable/completion_queue.h"
#include "google/cloud/bigtable/internal/async_loop_op.h"
#include "google/cloud/bigtable/internal/async_op_traits.h"
#include "google/cloud/bigtable/metadata_update_policy.h"
#include "google/cloud/bigtable/rpc_backoff_policy.h"
#include "google/cloud/bigtable/rpc_retry_policy.h"
#include "google/cloud/bigtable/version.h"
#include "google/cloud/internal/make_unique.h"
#include "google/cloud/internal/throw_delegate.h"

namespace google {
namespace cloud {
namespace bigtable {
inline namespace BIGTABLE_CLIENT_NS {
namespace internal {

/**
 * A dummy function object only to ease specification of retriable operations.
 *
 * It is an example type which could be passed to `Start()` member function of
 * the operation to be retried.
 */
struct PrototypeStartCallback {
  void operator()(CompletionQueue&, grpc::Status&) const {}
};

/**
 * A check if the template parameter meets criteria for `AsyncRetryOp`.
 */
template <typename Operation>
struct MeetsAsyncOperationRequirements {
  static_assert(HasStart<Operation, PrototypeStartCallback>::value,
                "Operation has to have a templated Start() member function "
                "instantiatable with functors like PrototypeStartCallback");
  static_assert(
      HasAccumulatedResult<Operation>::value,
      "Operation has to have an AccumulatedResult() member function.");
  static_assert(
      google::cloud::internal::is_invocable<
          decltype(&Operation::template Start<PrototypeStartCallback>),
          Operation&, CompletionQueue&, std::unique_ptr<grpc::ClientContext>&&,
          PrototypeStartCallback&&>::value,
      "Operation::Start<PrototypeStartCallback> has to be "
      "non-static and invocable with "
      "CompletionQueue&, std::unique_ptr<grpc::ClientContext>&&, "
      "PrototypeStartCallback&&.");
  static_assert(google::cloud::internal::is_invocable<
                    decltype(&Operation::AccumulatedResult), Operation&>::value,
                "Operation::AccumulatedResult has to be non-static and "
                "invokable with no arguments");
  static_assert(
      std::is_same<
          google::cloud::internal::invoke_result_t<
              decltype(&Operation::template Start<PrototypeStartCallback>),
              Operation&, CompletionQueue&,
              std::unique_ptr<grpc::ClientContext>&&, PrototypeStartCallback&&>,
          std::shared_ptr<AsyncOperation>>::value,
      "Operation::Start<>(...) has to return a "
      "std::shared_ptr<AsyncOperation>");

  using Response = google::cloud::internal::invoke_result_t<
      decltype(&Operation::AccumulatedResult), Operation&>;
};

/**
 * An idempotent policy for `AsyncRetryOp` based on a pre-computed value.
 *
 * In most APIs the idempotency of the API is either known at compile-time or
 * the value is unchanged during the retry loop. This class can be used in
 * those cases as the `IdempotentPolicy` template parameter for
 * `AsyncRetryOp`.
 */
class ConstantIdempotencyPolicy {
 public:
  explicit ConstantIdempotencyPolicy(bool is_idempotent)
      : is_idempotent_(is_idempotent) {}

  bool is_idempotent() const { return is_idempotent_; }

 private:
  bool is_idempotent_;
};

}  // namespace internal
}  // namespace BIGTABLE_CLIENT_NS
}  // namespace bigtable
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_BIGTABLE_INTERNAL_ASYNC_RETRY_OP_H_
