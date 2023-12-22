import router from '@ohos.router'
import hilog from '@ohos.hilog';

const TAG = '[IndexPage]';

@Entry
@Component
struct IndexPage {
  @State message: string = 'Index Page'

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize('38fp')
          .fontWeight(FontWeight.Bold)
        Blank()
        Button("Next")
          .fontSize('16fp')
          .width('296vp')
          .height('40vp')
          .backgroundColor('#007DFF')
          .onClick(() => {
            router.pushUrl({
              url: 'pages/SecondPage',
              params: {
                src: 'Index页面传来的数据'
              }
            }).then(() => {
              console.info('Succeeded in jumping to the second page.')
            }).catch((error: Error) => {
              hilog.debug(0, TAG, 'IndexPage push error' + JSON.stringify(error))
            })
          })

        Image($r('app.media.test'))
          .objectFit(ImageFit.Contain)
          .borderRadius(200)
          .clip(true)
          .width('100vp')
          .height('100vp')
          .margin($r('app.float.checkbox_margin'))
      }
      .width('100%')
    }
    .height('100%')
  }
}