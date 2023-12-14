import { RequestOption } from '@ohos/imageknife/src/main/ets/components/imageknife/RequestOption'
import { ImageKnife, ImageKnifeComponent, ImageKnifeOption, ImageKnifeGlobal, ImageKnifeData, ImageKnifeDrawFactory, BlurTransformation } from '@ohos/imageknife'
import { BusinessError } from '@ohos.base'
import request from '@ohos.request'


//let mUrl = "https://hbimg.huabanimg.com/cc6af25f8d782d3cf3122bef4e61571378271145735e9-vEVggB"
let mUrl = $r('app.media.test');

@Entry
@Component
struct TestSimpleOptionPage {
  @State imageKnifeOption1: ImageKnifeOption = {
    // 加载一张本地的jpg资源（必选）
    loadSrc: $r('app.media.test'),
    // 绘制圆角30，边框5，边框"#ff00ff".用户自定义绘制（可选）
    drawLifeCycle:ImageKnifeDrawFactory.createRoundLifeCycle(1,"#00ff00ff",30)
  }

  @State mBlurPixelMap?: PixelMap = undefined;

  build() {
    Scroll() {
      Flex({direction: FlexDirection.Column, alignItems:ItemAlign.Center, justifyContent: FlexAlign.Center}) {
        ImageKnifeComponent({imageKnifeOption: this.imageKnifeOption1})
          .width(300)
          .height(300)

        Button('预加载本地资源gif')
          .onClick(() => {
            let request = new RequestOption();
            request.load($r('app.media.test'))
              .setImageViewSize({width: 300, height: 300})
              .addListener({callback: (err: BusinessError|string, data:ImageKnifeData) => {
                if (err) {
                  console.log('预加载本地资源gif 出现错误! err=' + err)
                } else {
                  console.log('预加载本地资源gif成功! imageKnifedata=' + JSON.stringify(data))
                }
                return false;
              }})
            ImageKnifeGlobal.getInstance().getImageKnife()?.preload(request);
          })
          .margin({left: 15})
          .backgroundColor(Color.Grey);

        Text("BlurTransformation").fontColor(Color.Gray).fontSize(16);
        Button() {
          Text("高斯模糊").fontSize(13).fontColor(Color.White)
        }
        .height(35)
        .width(120)
        .margin({ top: 10 })
        .onClick(() => {
          this.blurHandlePixelMap(20);
        });
        // Image(this.mBlurPixelMap==undefined?'':this.mBlurPixelMap!)
        //   .objectFit(ImageFit.Fill)
        //   .width(200)
        //   .height(200)
        //   .margin({ top: 10 })

        ImageKnifeComponent({imageKnifeOption: {
          loadSrc: this.mBlurPixelMap==undefined?'':this.mBlurPixelMap!
        }})
          .width(200)
          .height(200)

      }.margin({ top: 10 });
    }.width('100%')
    .height('100%')
  }

  /**
   *模糊
   */
  blurHandlePixelMap(radius: number) {
    let imageKnifeOption = new RequestOption();
    let transformation = new BlurTransformation(radius);
    imageKnifeOption.load(mUrl)
      .addListener({callback:(err:BusinessError|string, data:ImageKnifeData) => {

        this.mBlurPixelMap = data.drawPixelMap?.imagePixelMap as PixelMap;

        return false;
      }})
      .setImageViewSize({ width: vp2px(200), height: vp2px(200) })
      .skipMemoryCache(true)
      .enableGPU()
      .blur(radius)
    ImageKnifeGlobal.getInstance().getImageKnife()?.call(imageKnifeOption);
  }
}