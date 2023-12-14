// import { ImageKnifeComponent } from '@ohos/imageknife/src/main/ets/components/imageknife/ImageKnifeComponent';
import { ImageKnifeComponent, ImageKnifeOption, ImageKnife, ImageKnifeGlobal, ScaleType, DiskStrategy,
  AUTOMATIC,
  TransformType,
  RotateImageTransformation,
  AllCacheInfo,
  ObjectKey,
  ImageKnifeDrawFactory
} from '@ohos/imageknife'
import worker from '@ohos.worker'

@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  //加载本地图片
  @State option: ImageKnifeOption = {
    loadSrc: $r('app.media.icon')
  }
  // 加载网络静图
  @State jpgUrl: string = 'https://p6-tt.byteimg.com/tos-cn-i-0000/7bc94ac9f7574a35b5a3fc5078172ddc~640x0.jpeg'
  @State option2: ImageKnifeOption = {
    loadSrc: this.jpgUrl
  }

  @State gifOption1: ImageKnifeOption = {
    loadSrc: $r('app.media.icon_loading'),
    placeholderSrc: $r('app.media.icon_loading'),
    errorholderSrc: $r('app.media.icon_failed')
  }
  //罗列ImageKnifeOption的各属性配置
  @State demoOption: ImageKnifeOption = {
    loadSrc: $r('app.media.icon_loading'),
    mainScaleType: ScaleType.FIT_CENTER,
    strategy: new AUTOMATIC(),
    dontAnimateFlag: false,
    placeholderSrc: $r('app.media.icon_loading'),
    placeholderScaleType: ScaleType.FIT_XY,
    errorholderSrc: $r('app.media.icon_failed'),
    errorholderSrcScaleType:ScaleType.FIT_START,
    retryholderScaleType: ScaleType.FIT_END,
    thumbSizeMultiplier: 0.5,
    thumbSizeDelay:1000,
    thumbSizeMultiplierScaleType:ScaleType.FIT_CENTER,
    displayProgress: true,
    canRetryClick:false,
    onlyRetrieveFromCache:true,
    isCacheable:false,
    transform: {
      transformType: TransformType.RotateImageTransformation,
      rotateImage: 180
    },
    transformations:[new RotateImageTransformation(180)],
    allCacheInfoCallback:{callback:(allCacheInfo:AllCacheInfo)=>{
      let info = allCacheInfo as AllCacheInfo;
      console.log("AllCacheInfoCallback imageknifecomponent1 memory ="+JSON.stringify(info.memoryCacheInfo))
      console.log("AllCacheInfoCallback imageknifecomponent1 resource ="+JSON.stringify(info.resourceCacheInfo))
      console.log("AllCacheInfoCallback imageknifecomponent1 data ="+JSON.stringify(info.dataCacheInfo))
    }
    },
    signature:new ObjectKey(new Date().getTime().toString()),
    drawLifeCycle:ImageKnifeDrawFactory.createOvalLifeCycle(15,'#ff000000')

    // gif: {
    //   loopFinish:{loopTime:2000},
    //   speedFactory:2,
    //   seekTo:3
    // }
  }

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(30)
          .fontWeight(FontWeight.Bold)

        Flex({direction:FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent:FlexAlign.Center}) {
          ImageKnifeComponent({imageKnifeOption: this.option})
            .width(100)
            .height(100)

          ImageKnifeComponent({imageKnifeOption: this.option2})
            .width(200)
            .height(200)
        }

        Text("加载一张网络gif图片").fontSize(15)
        Text('gif解析在子线程，请在页面构建后创建worker，注入imageKnife').fontSize(15)
        Flex({ direction: FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center}) {
          Button('加载Gif')
            .onClick(() => {
              this.gifOption1 = {
                loadSrc: 'https://gd-hbimg.huaban.com/e0a25a7cab0d7c2431978726971d61720732728a315ae-57EskW_fw658',
                placeholderSrc: $r('app.media.icon_loading'),
                errorholderSrc: $r('app.media.icon_failed'),
                displayProgress: true
              }
            }).margin({top: 5, left: 3})
          ImageKnifeComponent({imageKnifeOption: this.gifOption1}).width(200).height(200)
        }.width("100%").backgroundColor(Color.Pink)
      }
      .width('100%')
    }
    .height('100%')
  }

  aboutToAppear() {
  }

  aboutToDisappear() {
  }
}