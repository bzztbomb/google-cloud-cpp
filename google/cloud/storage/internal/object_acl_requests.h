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

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_ACL_REQUESTS_H_
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_ACL_REQUESTS_H_

#include "google/cloud/storage/internal/generic_object_request.h"
#include "google/cloud/storage/internal/http_response.h"
#include "google/cloud/storage/object_access_control.h"
#include "google/cloud/storage/well_known_parameters.h"
#include <iosfwd>

namespace google {
namespace cloud {
namespace storage {
inline namespace STORAGE_CLIENT_NS {
namespace internal {
/// A request type for `ObjectAccessControl: {get,delete,patch,update}`
class ObjectAclRequest
    : public GenericObjectRequest<ObjectAclRequest, Generation, UserProject> {
 public:
  ObjectAclRequest() : GenericObjectRequest() {}
  ObjectAclRequest(std::string bucket, std::string object, std::string entity)
      : GenericObjectRequest(std::move(bucket), std::move(object)),
        entity_(std::move(entity)) {}

  std::string const& entity() const { return entity_; }
  ObjectAclRequest& set_entity(std::string v) {
    entity_ = std::move(v);
    return *this;
  }

 private:
  std::string entity_;
};

std::ostream& operator<<(std::ostream& os, ObjectAclRequest const& r);

}  // namespace internal
}  // namespace STORAGE_CLIENT_NS
}  // namespace storage
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_ACL_REQUESTS_H_