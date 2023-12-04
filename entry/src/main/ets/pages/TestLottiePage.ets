import lottie, { AnimationItem } from '@ohos/lottie';
import { CommonConstants } from '../common/constants/CommonConst';

@Entry
@Component
struct TestLottiePage {
  @State message: string = '测试lottie动画';

  private renderingSettings: RenderingContextSettings = new RenderingContextSettings(true);
  private renderingContext: CanvasRenderingContext2D = new CanvasRenderingContext2D(this.renderingSettings);
  private animateName: string = CommonConstants.ANIMATE_NAME;
  private animateItem: AnimationItem | null = null;
  @State canvasTitle: Resource | undefined = undefined;

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(20)
          .fontWeight(FontWeight.Bold)

        Blank()
        Canvas(this.renderingContext)
          .width(CommonConstants.CONTAINER_WIDTH)
          .aspectRatio(CommonConstants.ASPECT_RATIO_176)
          .backgroundImage($r('app.media.canvasBg'))
          .backgroundImageSize(ImageSize.Cover)
          .onDisAppear(() => {
            lottie.destroy(this.animateName);
          })
        Button() {
          Text($r('app.string.outer_button_load'))
            .fontSize($r('app.float.fontSize_16'))
            .fontColor($r('app.color.outer_button_font'))
            .fontWeight(FontWeight.Bold)
        }
        .width(CommonConstants.CONTAINER_WIDTH)
        .height($r('app.float.default_40'))
        .backgroundColor($r('app.color.outer_button_bg'))
        .onClick(() => {
          if (this.animateItem !== null) {
            this.animateItem.destroy();
            this.animateItem = null;
          }
          this.canvasTitle = $r('app.string.outer_button_load');
          this.animateItem = lottie.loadAnimation({
            container: this.renderingContext,
            renderer: 'canvas',
            loop: 10,
            autoplay: true,
            name: this.animateName,
            path: 'common/lottie/data.json'
          });
        })
      }
      .width('100%')
    }
    .height('100%')
  }
}