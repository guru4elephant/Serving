// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
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

#pragma once
#include <string>
#include <vector>
#include "examples/demo-serving/image_class.pb.h"
#include "core/predictor/builtin_format.pb.h"
#include "core/predictor/common/inner_common.h"
#include "core/predictor/framework/channel.h"
#include "core/predictor/framework/op_repository.h"
#include "core/predictor/op/op.h"

// opencv
#include "opencv/cv.h"
#include "opencv/cv.hpp"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "paddle_inference_api.h"  // NOLINT

namespace baidu {
namespace paddle_serving {
namespace serving {

struct ReaderOutput {
  std::vector<paddle::PaddleTensor> tensors;

  void Clear() {
    size_t tensor_count = tensors.size();
    for (size_t ti = 0; ti < tensor_count; ++ti) {
      tensors[ti].shape.clear();
    }
    tensors.clear();
  }

  std::string ShortDebugString() const { return "Not implemented!"; }
};

class ReaderOp
    : public baidu::paddle_serving::predictor::OpWithChannel<ReaderOutput> {
 public:
  typedef std::vector<paddle::PaddleTensor> TensorVector;

  DECLARE_OP(ReaderOp);

  int inference();

 private:
  cv::Mat _image_8u_tmp;
  cv::Mat _image_8u_rgb;
  std::vector<char> _image_vec_tmp;
};

}  // namespace serving
}  // namespace paddle_serving
}  // namespace baidu
