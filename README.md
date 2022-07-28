# DigitalEarth

## osgearth介绍 ##

    说明：
    1. 运行环境win7 x64 旗舰版，CPU i5-6400 8核，8G内存；
    2. 括号内是对应demo的测试参数(.earth)；
    3. 配置OSG_FILE_PATH；
    4. 参数命令行可以查看各个demo需要的输入参数；
    5. 前缀说明：gui界面程序、cmd控制台程序、non功能未知；
    6. 程序后括号里的non表示不需要通过命令行输入参数；

## 示例： ##

    1、gui:osgearth_3pv(simple.earth)：相机与地球的投影关系，Range表示相机到鼠标在地球上投影点的距离； 
    2、gui:osgearth_annotation(annotation.earth)：演示各类标注（点、线、面、模型、椭圆、文本、图片等）；
    3、cmd:osgearth_atlas(--build data\resources\textures_us\catalog.xml)：创建地图册，在当前目录下生成catalog_atlas.osgb和catalog_atlas.osgb.xml文件，catalog_atlas.osgb在osgviewer工具加载失败；
    4、cmd:osgearth_boundarygen(cow.osg)：边界生成，在当前目录生成boundary.txt的边界文件（可通过--out指定输出的路径的文件名）；
    5、non:osgeaurth_cache()：解析带cache的earth文件中的cache list，通过参数--list *.earth文件（未找到带有cache的earth文件）；
    6、non:osgearth_city(boston.earth)：加载显示一个城市三维模型，demo源码中让输入boston.earth文件，但没有加载出来；
    7、gui:osgearth_cluster(simple.earth)：演示创建集群对象的功能；
    8、gui:osgearth_colorfilter(simple.earth --rgb)：演示颜色过滤功能；
    9、non:osgearth_computerangecallback(simple.earth)：演示；
    10、gui:osgearth_controls(simple.earth)：演示添加控制界面（复选框、按钮、滑动条）；
    11、cmd:osgearth_conv()：转换GIS的瓦片数据，研究命令行参数；
    12、non:osgearth_datetime
    13、gui:osgearth_drawables(non):基本绘制效果、形状；
    14、gui:osgearth_eci(simple.earth): 地心惯性坐标系（ECI）示例，可以拖动时间轴查看效果；
    15、gui:osgearth_elevation(feature_elevatiom.earth):通过左下角的“1Mt Rainier”定位，需要多点几次才能显示出高度信息；
    16、gui:osgearth_ephemeris（simple.earth --sky）:演示星历相关的效果和功能；
    17、gui:osgearth_featurefilter(feature_custom_filters.earth):演示要素的过滤效果；
    18、non:osgearth_featureinfo():显示要素信息；
    19、non:osgearth_featurequery():查询地图要素;
    20、gui:osgearth_features(non):显示地图矢量要素,显示国界线要素;
    21、gui:osgearth_geodetic_graticule(simple.earth):经纬度网络及对应的标签显示隐藏控制；
    22、gui:osgearth_graticule(simple.earth):经纬度网络及鼠标位置下的经纬度实时显示；
    24、gui:osgearth_horizon(simple.earth --activity):绘制一个半球(sphere);
    25、gui:osgearth_htm(simple.earth --model cow.osg):模型未正确加载;
    26、gui:osgearth_http(non):访问https://readymap.org/地图，测试时这个址打不开；
    27、gui:osgearth_imageoverlay(simple.earth):演示图片的覆盖效果，可以调整图片的透明度；
    28、gui:osgearth_infinitescroll(annotation_dateline_projected.earth):演示投影地图的无限次滑动；
    29、gui:osgearth_lights(simple.earth):演示灯效果；
    30、gui:osgearth_los(simple.earth):演示模型绕点旋转、线、圆效果；
    31、gui:osgearth_magnify(simple.earth):演示两个地图同步操作及单个地球的放大功能；
    32、gui:osgearth_manip(simple.earth):演示通过鼠标键盘操作地球功能；
    33、gui:osgearth_map(non):演示地图及地图图层相关功能；
    34、gui:osgearth_measure(simple.earth):演示距离测量功能；
    35、gui:osgearth_minimap(simple.earth):演示地球和地球投影效果；
    36、gui:osgearth_mrt(simple.earth):演示MRT渲染；
    37、non:osgearth_noisegen():
    38、non:osgearth_occlusionculling(feature_occlusion_culling.earth):演示遮挡剔除效果；
    39、gui:osgearth_overlayviewer(simple.earth):用于调试覆盖装饰器功能。它显示了两个窗口，一个窗口具有地图的法向视图，另一个窗口显示用于覆盖计算的边界截锥
    40、cmd:osgearth_package(--tms file.earth --out package):创建一个可再发行的 TMS 基于地球文件的包
    41、gui:osgearth_pick(annotation.earth):演示对象拾取功能；
    42、non:osgearth_scenegraphcallbacks():
    43、non:osgearth_sequencecontrol():
    44、non:osgearth_shadercomp
    45、non:osgearth_shadergen
    46、gui:osgearth_skyview(skyview1.earth):演示天空视图
    47、gui:osgearth_splat(non):木板
    48、cmd:osgearth_srstest(non):坐标参考系统转换；
    49、gui:osgearth_terrainprofile(simple.earth):演示地形纵断面;
    50、cmd:osgEarth_tests():测试程序，对osgearth本身的测试；
    51、cmd:osgearth_tfs(world.shp):将shp文件转换成tfs结构数据；
    52、non:osgearth_tileindex
    53、gui:osgearth_tilesource(non):演示地球的瓦片源（瓦片网格）；
    54、gui:osgearth_toc(simple.earth):演示地图图层的添加、删除、隐藏、显示、透明度；
    55、gui:osgearth_tracks(simple.earth):演示对象各种运动效果；
    56、gui:osgearth_transform(simple.earth):演示模型的transform;
    57、cmd:osgearth_version(non):输出当前版本；
    58、non:osgearth_video(non):未看到效果；
    59、gui:osgearth_viewer(*.earth):earth文件查看工具；
    60、non:osgearth_wfs(non):程序未运行起来；
    61、gui:osgearth_windows(simple.earth):演示窗口；
    