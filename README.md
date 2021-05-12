​	本程序实现了COVID-19疫情旅行模拟系统，有手动输入和读取真实时刻、地图信息输入两种输入数据的方式。能够按照起点、终点、出发时间范围和最少风险策略，限时最少风险策略规划旅行路线。

​	本程序规划好的路线需要输出给用户，包括起点出发的时间，中转城市到达时间，再次出发时间，和终点城市的到达时间等信息；如果有多条线路符合要求，都输出给用户。

​	本程序实现了：

（a）能够实现系统时钟的推进；

（b）系统时间至少精确到小时；

（c）日志文件记录旅客状态变化；

（d）能够查询不同旅客的当前位置信息；

（e）在旅客进入旅行路线后，可以更改路线；

（f）使用地图反应出旅行过程；

（g）能够按照起点、终点、出发时间范围、途经某些城市和任一旅行策略规划旅行路线。

（h）其他额外的功能：新建旅客，新建城市，添加/修改路径，时刻表。

# 用户使用说明与测试样例 

## 目录

**一、** **数据的输入**

1.1读取数据文件

1.2 手动输入

**二、模拟旅行**

2.1界面说明

2.2添加城市

2.3添加/修改时刻表

2.4详细查询

2.5添加旅客

2.6修改旅客旅行计划

**三、数据文件的编写**

##### 四、测试样例与分析

4.1最小风险策略测试及验证

4.2限制必经城市的路径规划测试及验证

4.3有时间限制的路径规划测试及验证



## 一、数据的输入

#### 1.1读取数据文件

1. 点击运行可执行文件，点击主界面上的“读取真实数据”按钮。

![image](https://user-images.githubusercontent.com/55476033/117961126-5634f980-b350-11eb-97d1-9dcf1da51d96.png)

**注意：**读取数据文件之前，请确保可执行文件与RealData.txt在同一目录下，若数据文件放错位置，会出现如下提示：

![image](https://user-images.githubusercontent.com/55476033/117961137-59c88080-b350-11eb-8a52-6e933f952dc8.png)

2. 成功找到数据文件后，会弹出对话框，请输入文件中各城市的风险值。**


![image](https://user-images.githubusercontent.com/55476033/117961155-5f25cb00-b350-11eb-8403-0a3a3c736841.png)

如果风险值输入错误，可以按“cancel”键重新输入，输入确认无误后点击“OK”进行下一个城市风险的输入。下图为点击“cancel”后的提示：

 
![image](https://user-images.githubusercontent.com/55476033/117961168-64831580-b350-11eb-97d5-058ac1fe2587.png)
 

**注意：**此处的输入有范围限制，只接受0.2-0.9之间的一位小数，如果试图输入不符合规定的数字是无法输入成功的。

 

3. 当全部城市的风险值输入完成后，会出现下图所示的提示框，返回主界面。**

 

![image](https://user-images.githubusercontent.com/55476033/117961184-68af3300-b350-11eb-8c65-348b23266a96.png)
 

#### 1.2 手动输入

在此部分的使用说明中，我会给出一个十分简单的输入样例来进行讲解。输入样例如下：

“地图上只有北京（1号）和呼和浩特（2号）两个城市，各自的风险皆为0.2，从呼和浩特到北京每天只有一班汽车，无火车与飞机线路。汽车每日零点发车，耗时一小时到达北京。无预输入的乘客。”（乘客信息的输入方法我在“2.2 添加旅客”中统一说明）

下面开始输入此例：

 

（1）   **点击主界面上的“手动输入按钮”进入手动输入模式。**

![image](https://user-images.githubusercontent.com/55476033/117961219-72d13180-b350-11eb-935f-04014bf649bb.png)


（2）   **首先创建地图信息，输入城市总数，汽车线路数，火车线路数，飞机线路数。**

 

**注意：**此线路数为两城市之间的直接通路数，不是交通工具班次数量，比如：A到B每日有十班汽车，十班火车，十班飞机通勤，这只能算一条汽车线路，一条火车线路和一条飞机线路。若某一个对话框种不填任何数字，软件默认按0处理。

 

![image](https://user-images.githubusercontent.com/55476033/117961251-7e245d00-b350-11eb-992b-fa6fa01ee358.png)
 

**注意：**城市总数不得超过31，不得输入负数，各线路数不得超过 N x (N - 1)（N为城市数，即每两个城市都双向互联），如果输入不合法，会弹出如下提示框，并清空此页数据，重新输入：

 

![image](https://user-images.githubusercontent.com/55476033/117961267-81b7e400-b350-11eb-89ba-b47084c44e3d.png)
​     

按照我们的输入样例，此处我们应该分别输入：“2 1 0 0”（0可以省略不填）。输入完成后点击确认。

 

（3）   **输入城市的风险值。**

当确定城市数后，首先弹出下面的对话框，同读取数据模式一样，点击“OK”进入下一个城市风险值的输入，点击“cancel”重新输入。输入各城市的风险值，城市与序号的对应关系在界面右下角有给出，也可以参考《功能需求分析与总体方案设计》的3.2.1节。按照讲解案例，此处都输入0.2。

![image](https://user-images.githubusercontent.com/55476033/117961279-854b6b00-b350-11eb-9187-f25765060dc4.png)

![image](https://user-images.githubusercontent.com/55476033/117961287-87adc500-b350-11eb-8b61-8c69e862a012.png)

注意：同读取数据模式，此处的输入有范围限制，只接受0.2-0.9之间的一位小数，如果试图输入不符合规定的数字是无法输入成功的。

 

（4）   **开始创建汽车线路图（火车，飞机线路图的创建同理）。**

 

界面如下所示：

![image](https://user-images.githubusercontent.com/55476033/117961299-8b414c00-b350-11eb-9823-d92f6785de75.png)
 

按照我们的讲解案例，此处应该依次填入“2 1 1”，点击确定。如果任意一个空填入了负数、不填写任何数值，或者起始城市或终止城市序号超过之前输入的城市总数的话，会出现如下报错提示：

![image](https://user-images.githubusercontent.com/55476033/117961317-8f6d6980-b350-11eb-8202-54399662eac2.png)
 

（5）   **构建车次表。**

 

首先输入的是两城市之间的交通工具的发车次数。具体输入的是从哪个城市到哪个城市的何种交通工具的发车次数，对话框都会清楚地提示出，此例中输入框如下所示。按照我们的讲解案例，此处应该输入“1”。输入完成后点击”OK”。（此输入框的默认值即为1）

![image](https://user-images.githubusercontent.com/55476033/117961327-93998700-b350-11eb-9000-0774aa806123.png)
 

如果点击取消，则会提示重新输入。

![image](https://user-images.githubusercontent.com/55476033/117961334-95fbe100-b350-11eb-82e8-c850fefc8088.png)

输入完两城市间车次后，会出现提示框，要求输入第N班车次的首发时刻，按照我们的讲解案例，此处输入“0”。（此输入栏默认值为0）

 

![image](https://user-images.githubusercontent.com/55476033/117961351-998f6800-b350-11eb-8055-48b668cc08f3.png)
 

**注意：**此处的对话框输入也是有范围限制的，只接受0-23的整数。输入不符合格式会无法输入成功。

 

（6）   **创建初始旅客信息。**

 

在构建完交通工具时刻表之后，会进入创建初始旅客信息的界面，设计这个界面是因为这样可以使得旅客在系统运行的开始时刻（第0天第0时）就出发成为可能。按照我们的讲解案例，此处之间输入0即可。（此处的默认值即为0）

 
![image](https://user-images.githubusercontent.com/55476033/117961366-9d22ef00-b350-11eb-8e0b-ae6da1f5e400.png)
 

至此手动输入模式完成，出现如下提示回到主界面。

 

![image](https://user-images.githubusercontent.com/55476033/117961380-a0b67600-b350-11eb-85e4-35b83654cafd.png)


**注意：**如果已经手动输入了数据再点击数据读入或者已经读入了数据文件再点击手动输入案件会出现如下的提示：

![image](https://user-images.githubusercontent.com/55476033/117961393-a57b2a00-b350-11eb-8f23-cf7bad19d1d4.png)
 

## 二、   模拟旅行

在此部分，我会使用RealData.txt中的数据进行讲解。各城市的风险值我会随机输入。读取完RealData.txt中的数据并输入完城市风险值后，界面如下所示。

#### 2.1界面说明

![image](https://user-images.githubusercontent.com/55476033/117961409-aad87480-b350-11eb-938a-bf31767ce8e7.png)

**（1）**   **时间显示区**

在界面的左上角，是模拟系统时间显示区，显示了当前模拟系统的时间。

**（2）**   **按键区**

按键区为用户提供了五个功能按键：“详细查询”，“添加城市”，“添加班次”，“添加旅客”，“修改路径”。点击对应的图标可以执行相应的操作。

**（3）**   **状态日志区**

这里显示了当前时刻发生的事件，比如旅客状态信息，用户的操作等等。与“详细查询”的区别是这里的事件描述的很简略，也不能查看交通工具班次与时刻表。

**（4）**   **城市与序号对应关系表**

这一区域主要是为了手动添加班次时，使用户查看到城市与序号的对应关系。

**（5）**   **直达路线**

两城市之间的直达路线在图上由虚线标记出，其中，蓝色的虚线表示飞机线路，绿色虚线表示火车线路，黄色虚线表示汽车线路。

**（6）**   **城市标志**

当前已设置的城市会由彩色的圆点标记出。如图所示，红色标记的城市表示风险值为0.9，黄色标记的城市表示风险值为0.5，绿色标记的城市风险值为0.2。

**（7）**   **交通工具标志**

当旅客在旅行时，地图上会实时呈现出旅客的位置，旅客乘坐的交通工具如下所示：


![image](https://user-images.githubusercontent.com/55476033/117961466-bc218100-b350-11eb-94f0-33011c64912c.png)![image](https://user-images.githubusercontent.com/55476033/117962765-2be43b80-b352-11eb-916c-6f902289ec55.png)![image](https://user-images.githubusercontent.com/55476033/117962784-30105900-b352-11eb-99d1-c886c031400f.png)

汽 车         火 车        飞 机

 

#### 2.2 添加城市

只需点击添加城市键就可以进行添加城市操作。当系统中已经存在了X（X <31）个城市后，点击添加城市键会进行第X + 1号城市的风险值输入，输入完成即添加成功。在文件中，我给出了30个城市的信息，现在我来添加第31号城市，武汉。  

 

![image](https://user-images.githubusercontent.com/55476033/117961514-c80d4300-b350-11eb-9b83-99e30c32dd15.png)
 


设武汉的风险值为0.2。点击OK即输入完成。

**注意：**此对话框与前一章的风险输入对话框不同，点击cancel不会重新输入，而是放弃输入，回到运行界面。

 

![image](https://user-images.githubusercontent.com/55476033/117961526-cba0ca00-b350-11eb-8400-91b4d4c2fc04.png)


如上图，武汉的城市被绿色圆形标记，城市添加成功。此时城市已达到最大数目，如果再点击添加城市键会出现如下报错提示：

![image](https://user-images.githubusercontent.com/55476033/117961538-ce032400-b350-11eb-9e2c-999d36887a13.png)
 

#### 2.3添加/修改时刻表

现在武汉市已经被添加到了地图上，但是没有通路与外界相连，现在我们来添加路线使其与外界相连。

点击添加班次按键，进入如下界面：

 

![image](https://user-images.githubusercontent.com/55476033/117961560-d196ab00-b350-11eb-956d-c691789743c4.png)



在这里我添加一条武汉到长沙的火车线路，耗时2小时；一条武汉到北京的飞机线路，耗时3小时，那么在对话框中分别输入“0 1 1”（0可以不输入）。

首先进入火车线路创建界面，由界面上的城市与序号对照表可知，输入应为“31 27 2”，点击确定后，输入每日发车次数与首发时间，输入方法与第一节中所述一致，在此我输入的是“1班火车，每日12点首发”


![image](https://user-images.githubusercontent.com/55476033/117961576-d78c8c00-b350-11eb-8641-d9d065174ea4.png)
 

接下来进入飞机线路创建界面，按照样例输入应如下所示，点击确定，输入飞机起飞次数和各班次首发时间，我输入的是每日两班，起飞时间分别是8点和16点。

 

![image](https://user-images.githubusercontent.com/55476033/117961584-dbb8a980-b350-11eb-9db1-de5ce4981d3b.png)
 


输入完成后回到运行主界面，发现路径已经添加成功，路径详细情况可以点击详细查询按钮查看，稍后介绍。    

![image](https://user-images.githubusercontent.com/55476033/117961591-dfe4c700-b350-11eb-893c-d3744ae403e2.png)

#### 2.4 详细查询

在上一节中，我们已经添加了两条路线，地图上可以直接看出效果，但是我们需要检验一下数据输入是否正确，这时可以点击详细查询按钮。下图是详细查询的界面：


![img]![image](https://user-images.githubusercontent.com/55476033/117961612-e5daa800-b350-11eb-962c-895b0695083f.png)


在左上的窗口中可以查询到每个城市的风险值，右上窗口可以查询到每个旅客选择的旅行计划详情，现在系统中还没有旅客，所以是空的。在左下窗口中可以查询到各种交通工具的所有时刻表，在右下的窗口中可以看到从0时刻开始各时刻旅客的状态，同样地，当前系统中没添加旅客，所以这里显示了时间，没有状态信息。

现在我们来检验一下在前一节输入的线路是否添加成功了。下拉时刻表窗口，查询到如下数据，说明我们的线路添加成功。

 

![image](https://user-images.githubusercontent.com/55476033/117961630-ea9f5c00-b350-11eb-968a-a87c701c02dd.png)
 

![image](https://user-images.githubusercontent.com/55476033/117961636-ed01b600-b350-11eb-9f37-c766e7e96929.png)
 

#### 2.5 添加旅客

​    现在为系统中添加旅客。在这里出于介绍方法的目的，不多添加，只添加一个旅客，使其在第四天的3点出发，起始城市设为哈尔滨，终点城市设为昆明。点击添加旅客，选择添加个数为1，进入旅客信息输入界面，我已经将数据输入进了对话框，界面如下所示。

![image](https://user-images.githubusercontent.com/55476033/117961648-f0953d00-b350-11eb-91e3-12f927063eee.png)

**注意：**此节目中的限制时间与必经城市都是可选项，当选中其勾选框时，激活后面的输入框，可以进行输入。旅客旅行计划的多种规划路径策略由这两个选项控制。通过勾选可以组合出以下四种策略。

（1）风险最小

（2）限时风险最小

（3）风险最小且有必经城市

（4）限时风险最小且有必经城市

​     出于演示作用，我在这里勾选一个限制时间选项，我设置其限制时间为50小时，点击确定。但是这时返回了一个错误，不存在这样的路径，这说明限时太少了。

![image](https://user-images.githubusercontent.com/55476033/117962001-5386d400-b351-11eb-9b1c-a326d86d116f.png)

于是我重新输入，取消勾选了限制时间，系统返回了如下的路径规划，我们可以看到，该计划花费了62小时，这也证明了，50小时的限时完成旅行确实是办不到的。

![image](https://user-images.githubusercontent.com/55476033/117962032-5a154b80-b351-11eb-8121-fa116430568f.png)
 

点击确定后，返回运行主界面开始模拟。这时我们来详细查询一下，看看旅客状态窗口和路径选择窗口的变化。

 

![image](https://user-images.githubusercontent.com/55476033/117962042-5da8d280-b351-11eb-83a8-c0b27982d1a3.png)

![image](https://user-images.githubusercontent.com/55476033/117962059-613c5980-b351-11eb-9bf6-b210adebb435.png)
 

可以看到，这两个窗口已经有旅客状态与计划的详细信息了。

 

![image](https://user-images.githubusercontent.com/55476033/117962065-65687700-b351-11eb-867c-fd2bafe4cb45.png)
 

如图所示，在第四天的6点，该旅客确实搭乘火车到达了长春。该路线规划的正确性与旅客后续状态的检验在此不赘述，我会放在“测试样例与分析”部分详述。

 

此外，值得一提的是，如果满足一名乘客的路径有多种，会弹出提示框，让用户选择一个方案。为了说明这个功能，我手动构建了如下的地图：

![image](https://user-images.githubusercontent.com/55476033/117962079-68fbfe00-b351-11eb-8ae9-aa25f8f63cc0.png)

在初始化时，我添加了一名乘客，使其从0天0时出发，从北京前往兰州。弹出的规划路径界面如下：

![image](https://user-images.githubusercontent.com/55476033/117962090-6c8f8500-b351-11eb-97b2-b9d0c6727fdc.png)

输入选择的路径再点击确认，即可。

#### **2.6** 修改旅客旅行计划

点击修改路径按钮，弹出如下提示框，输入一号。

 

![image](https://user-images.githubusercontent.com/55476033/117962115-72856600-b351-11eb-9cb5-ff4b1d0133dd.png)


点击OK后进入下面的界面：

 

![image](https://user-images.githubusercontent.com/55476033/117962136-774a1a00-b351-11eb-9f76-bf081b255ff5.png)


我们可以看到，在界面上，系统告诉了我们当前该旅客所处的城市为19，即长春。我们选择的出发起点即为长春，我们现在修改其目的地，此处我想让其目的地不再是昆明而是济南，则我设置目的地为15，出发时刻我设置为第四天的15点，不勾选限制时间与必经城市，点击确定。

系统返回了如下的路径，说明已经旅客的旅行计划修改成功。

![image](https://user-images.githubusercontent.com/55476033/117962149-7c0ece00-b351-11eb-9dc5-905d05625a20.png)
 

![image](https://user-images.githubusercontent.com/55476033/117962161-7f09be80-b351-11eb-8545-f8fbd11bfd94.png)
 

![image](https://user-images.githubusercontent.com/55476033/117962172-816c1880-b351-11eb-942d-ece96e5a37a3.png)
 

**注意：**修改旅客的旅行计划是由限制的，当旅客正在交通工具上时不能修改，当旅客已经结束旅行时不能修改，否则会分别出现以下的错误提示框。

 

![image](https://user-images.githubusercontent.com/55476033/117962181-84ff9f80-b351-11eb-83e5-7d529d2c8fd3.png)
 

![image](https://user-images.githubusercontent.com/55476033/117962189-87fa9000-b351-11eb-860e-9439440ed3b7.png)

 

## 三、数据文件的编写

在文件目录下有一个txt文本文件叫做“RealData.txt”，打开该文件，如下所示：

![image](https://user-images.githubusercontent.com/55476033/117962206-8c26ad80-b351-11eb-8583-66558c4cf2e7.png)

这就是本软件读取的文件了。为了方便读取，我没有使用文字，因为读取文件的过程是用户看不到的，此文件不需要对用户阅读友好，使用了ASCII码表示数据，这样还可以使读入数据的代码简洁，提高效率。在此软件中，31个我国主要城市在此文件中用ASCII码编码从“A”（北京）到“_”（武汉）。交通工具用“b”（bus）“t”（train）“p”（plane）表示。路程所需时间用阿拉伯数字表示，而每班车（飞机）每日的发车时刻用a ~ x（0 ~ 23）表示。数据格式为：

“起始城市ASCII码 终点城市ASCII码 交通工具 所需时间 首发时间”。

城市ASCII编码对照表如下：

| 北京     | A    | 杭州   | L    | 西安 | W    |
| -------- | ---- | ------ | ---- | ---- | ---- |
| 呼和浩特 | B    | 上海   | M    | 成都 | X    |
| 银川     | C    | 南京   | N    | 重庆 | Y    |
| 兰州     | D    | 济南   | O    | 贵阳 | Z    |
| 西宁     | E    | 天津   | P    | 长沙 | [    |
| 乌鲁木齐 | F    | 大连   | Q    | 南昌 | \    |
| 拉萨     | G    | 沈阳   | R    | 合肥 | ]    |
| 昆明     | H    | 长春   | S    | 郑州 | ^    |
| 南宁     | I    | 哈尔滨 | T    | 武汉 | _    |
| 广州     | J    | 石家庄 | U    |      |      |
| 福州     | K    | 太原   | V    |      |      |

 

现在让我以第一条数据“AFp4j”为例解读一下：从北京到乌鲁木齐的飞机，所需四小时，起飞时间为9点。对应的真实航班如下图（为了以小时为单位，对时刻进行了舍入）：

![image](https://user-images.githubusercontent.com/55476033/117962227-92b52500-b351-11eb-91c3-6c70c23f3e26.png)


相信看了这一部分的介绍，用户可以自己添加新的数据了。

**注意：**文件中回车不得单独一行。

## 四、测试样例

#### 4.1最小风险策略测试及验证

在这一个样例中，为了便于计算风险，我设置所有的城市风险值都为0.2，界面如下图所示，我添加的乘客为第0天3点从广州前往哈尔滨，选择最小风险策略，输入之后得到如下的路径规划：

![image](https://user-images.githubusercontent.com/55476033/117962249-98126f80-b351-11eb-9f17-a036e91ebc02.png)

经过我的程序计算，一共找出了3765条可能的路径，为了证明我的程序给出的这一种规划是风险最小的，我把这3765种可能的路径都输出到了“测试样例辅助文件1.txt”中，如下所示： 

![image](https://user-images.githubusercontent.com/55476033/117962309-aa8ca900-b351-11eb-8910-83493c3ca9f3.png)

​     经过查找，该文件中第一条的解法是风险最小的，如下所示：

![image](https://user-images.githubusercontent.com/55476033/117962317-ae203000-b351-11eb-803f-158423ab2ff7.png)

​     将序号换成对应的城市名，即程序给出的方案。现在让我们来验证一下这个结果的正确性：

该旅客第0天3点从广州出发，滞留九小时，即十二点乘坐飞机前往北京，经过查询，广州到北京的飞机的确是12点起飞。

 

![image](https://user-images.githubusercontent.com/55476033/117962335-b1b3b700-b351-11eb-9a89-6e31e91cc1bd.png)
 

在广州滞留了九小时，广州的风险值为0.2，那么离开广州时，累计的风险值应该是1.8，广州到北京的飞机时间为三小时，即15点到达北京，与计划所述一致，在飞机上累计的风险值为：

广州的风险值 x 飞机用时 x 飞机风险系数 = 0.2 x 3 x 9 =5.4

此时累计的风险值为1.8 + 5.4 = 7.2，与计划相符合。 

北京到沈阳只有飞机，所以我们来查询一下，发现北京到沈阳的飞机起飞时间为每日上午九点：

![image](https://user-images.githubusercontent.com/55476033/117962347-b5473e00-b351-11eb-9787-cbbc438cdaac.png)

​     旅客到达北京的时间为第0天15点，所以需要滞留18个小时才能乘坐第二天的飞机。

这与给出的规划相符合。这18个小时中，旅客的累计风险值增长了：18 x 0.2 = 3.6。乘坐飞机用时一小时到达沈阳，此时旅客的风险值增长了：

北京的风险值 x 飞机用时 x 飞机风险系数 = 0.2 x 1 x 9 = 1.8

​     所以当前累计风险值为 1.8 + 3.6 + 7.2 = 12.6。与给出的规划一致。

​     旅客到达沈阳的时间为第1天10点，但是经过查询发现沈阳到长春的发车时刻如下：

![image](https://user-images.githubusercontent.com/55476033/117962359-b9735b80-b351-11eb-9b17-9b29a2ec17b4.png)

​     在今天，沈阳到长春已经没有火车了，只能等到第2天的3点。在沈阳一共滞留了17个小时，增加了风险：0.2 x 17 = 3.4。乘坐三点的火车耗时三小时到达长春，此时风险增长了：

沈阳的风险值 x 火车用时 x 火车风险系数 = 0.2 x 3 x 5 = 3

此时累计风险值为：12.6 + 3 + 3.4 = 19，与计划相符合。

最后在第二天的六点，旅客到达了长春，查询长春到哈尔滨的火车发车时刻如下：

![image](https://user-images.githubusercontent.com/55476033/117962382-be380f80-b351-11eb-9136-cae19a7e9b06.png)

​     乘坐第一班八点的火车出发。共计在长春滞留2小时，增加风险0.4。在第二天的11点，旅客到达了哈尔滨，此时风险又增加了：

长春的风险值 x 火车用时 x 火车风险系数 = 0.2 x 3 x 5 = 3

累计风险值为：19 + 0.4 + 3 = 22.4。与计划相符合。

至此，我验证了我的程序搜索最小风险路径的正确性。

#### **4.2** 限制必经城市的路径规划测试及验证

依然使用上一节的数据，仍然使一个乘客从广州到哈尔滨，但是我设置必经城市为昆明，我得到了如下的规划：

![image](https://user-images.githubusercontent.com/55476033/117962407-c42df080-b351-11eb-850e-ef5473ffdecb.png)

​     同样地，我把所有的可能规划都放在了“”中，共计有832条规划，如下所示：

![image](https://user-images.githubusercontent.com/55476033/117962422-c7c17780-b351-11eb-825a-0b20a13c3898.png)

对比后可以发现，这832条路径全部囊括于之前3765条路径之中，只是都包括了必经城市昆明（8号城市）。

​     至于该样例的风险值，计算方法完全与第一节中的方法一致，我不再赘述。贴出该计划的运行界面图。

![image](https://user-images.githubusercontent.com/55476033/117962444-cee88580-b351-11eb-8232-435362a391ee.png)

​                               从广州乘汽车前往南宁

![image](https://user-images.githubusercontent.com/55476033/117962456-d314a300-b351-11eb-8c3c-741e77054005.png)

从南宁乘汽车前往贵阳

![image](https://user-images.githubusercontent.com/55476033/117962474-d6a82a00-b351-11eb-8265-a8d20b4e4b04.png)

​          从贵阳乘汽车前往昆明

![image](https://user-images.githubusercontent.com/55476033/117962492-dad44780-b351-11eb-9607-9b80af9e410e.png)

从昆明乘坐汽车前往成都

![image](https://user-images.githubusercontent.com/55476033/117962503-df98fb80-b351-11eb-9d61-a8b3219cddf4.png)

从成都乘飞机前往北京

![image](https://user-images.githubusercontent.com/55476033/117962519-e3c51900-b351-11eb-94a6-776e3a2a24af.png)

从北京乘飞机前往沈阳

![image](https://user-images.githubusercontent.com/55476033/117962539-e9bafa00-b351-11eb-89a8-89d54b9f53f8.png)![image](https://user-images.githubusercontent.com/55476033/117962552-ede71780-b351-11eb-8406-7b51010fcc41.png) 

乘坐火车从沈阳到长春，从长春到哈尔滨

![image](https://user-images.githubusercontent.com/55476033/117962563-f17a9e80-b351-11eb-8eb9-47f5c16b10e7.png)

最终状态

 

至此，我已经验证了限制必经城市时，规划路径算法的正确性。

#### **4.3** 有时间限制的路径规划测试及验证

​     由于之前的几个测试中，我使用的都是真实时刻表，两地之间一定是直达线路是最快的，这样也导致了使用这样的数据集时，找到的最小风险规划就是时间最短的规划。所以在这个测试中，我准备用手动输入，创造一个满足最小风险规划不是最短时间规划的情况，来测试及验证限制时间规划路径算法。

​     我通过手动输入构建出了如下的地图：

 

![image](https://user-images.githubusercontent.com/55476033/117962572-f50e2580-b351-11eb-94c0-4b8b1db4f0c3.png)

​     

在初始化时，我添加了两名旅客作为对比，第一名旅客从北京到兰州，限时12小时，第二名旅客从北京到兰州，不限时，系统给出的规划路径分别如下图所示。

![image](https://user-images.githubusercontent.com/55476033/117962584-f8a1ac80-b351-11eb-91cd-da209dbc794b.png)

![image](https://user-images.githubusercontent.com/55476033/117962600-fc353380-b351-11eb-8007-2e487ed3a60f.png)

​     

从上面两图中我们可以清楚地看到，限时旅行策略的作用。
