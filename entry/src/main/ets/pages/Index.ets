// import { ImageKnifeComponent } from '@ohos/imageknife/src/main/ets/components/imageknife/ImageKnifeComponent';
import { ImageKnifeComponent, ImageKnifeOption, ImageKnife, ImageKnifeGlobal } from '@ohos/imageknife'
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

  // 加载GIF动图
  private globalGifWorker?:worker.ThreadWorker = undefined
  @State gifOption1: ImageKnifeOption = {
    loadSrc: $r('app.media.icon_loading'),
    placeholderSrc: $r('app.media.icon_loading'),
    errorholderSrc: $r('app.media.icon_failed')
  }
  @State gifOption2: ImageKnifeOption = {
    loadSrc: $r('app.media.icon_loading'),
    placeholderSrc: $r('app.media.icon_loading'),
    errorholderSrc: $r('app.media.icon_failed')
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
                displayProgress: true,
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
    this.globalGifWorker = new worker.ThreadWorker('entry/ets/workers/GifLoadWorker.ts', {
      type: 'classic',
      name: 'ImageKnifeParserGif'
    })
    let imageKnife: ImageKnife|undefined = ImageKnifeGlobal.getInstance().getImageKnife()
    if (imageKnife != undefined) {
      imageKnife.setGifWorker(this.globalGifWorker)
    }
  }

  aboutToDisappear() {
    //页面销毁，销毁worker
    if (this.globalGifWorker) {
      this.globalGifWorker.terminate()
    }
  }
}