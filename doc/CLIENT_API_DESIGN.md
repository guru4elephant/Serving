## 模型无关的Serving和Client设计文档

### 实现目标：
- 一套Paddle Serving的动态库，支持Paddle保存的常规模型的远程预估服务。
- 提升Paddle Serving的易用性，给用户带来训练+远程预测的闭环体验

### 子目标：
- Client与Server可以通过rpc通信，通信格式可兼容各种模型的输入数据类型
- Client支持各种类型的输入数据设置，支持python和c++两种接口，需要有独立的client.so，暴露pybind并且有c++接口可以实现。

### 通用模型的范围：
- 能够使用Paddle Inference Library进行预测的模型
- 在训练过程中保存的模型，包含Feed Variable和Fetch Variable
- Feed Variable包含lod_level=1或者lod_level=0的情况，支持int64和float类型
- Fetch Variable包含lod_level=1或者lod_level=0的情况，支持float类型
- 用户在访问服务的过程中，可以在训练过程中保存的Variable集合中修改Fetch Variable的集合

### 整体设计：
- 用户通过Python Client启动Client和Server，Python API有检查互联和待访问模型是否匹配的功能
- Python API背后调用的是Paddle Serving实现的client和server对应功能的pybind，互传的信息通过RPC实现
- Client Python API当前有两个简单的功能，load_inference_conf和predict，分别用来执行加载待预测的模型和预测
- Server Python API主要负责加载预估模型，以及生成Paddle Serving需要的各种配置，包括engines，workflow，resource等



### RPC通信协议：
- Tensor可以兼容LodTensor(level=1)和n-d Tensor
- Tensor中的elem_type表示当前Tensor的数值类型，elem_type=0为int64，elem_type=1为float
- Tensor中的shape表示当前Tensor的形状，shape.size() >= 1，shape[0]=-1则表示当前Tensor为lod_level=1的LodTensor，否则表示n-d Tensor的实际shape
- FeedInst和FetchInst由若干Tensor组成，代表单条样本的多个输入或输出Variable
- Request和Response中包含若干FeedInst、FetchInst支持批量预测



### Server端支持通用模型加载的设计：
- 定义engine，workflow，resource，这部分会通过Server Python API自动生成，主要的变量就是model_data_path，需要加载当前实际需要预测的模型路径，对于workflow，由于当前Paddle Serving的设计可以服用已有workflow，因此可以不做自动生成



### Client端支持通用模型加载的设计：
- Client Python API加载模型需要加载一个训练过程中保存的模型配置，即inference model conf，包含用户预测过程中输入数据的具体Variable的信息
- 单个client可以连接多个server，在客户端可做负载均衡，数据并行预测
- 预测过程使用的reader，可以复用训练过程中的reader实现
