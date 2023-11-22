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
            }).catch((error: Error) => {
              hilog.debug(0, TAG, 'IndexPage push error' + JSON.stringify(error))
            })
          })
      }
      .width('100%')
    }
    .height('100%')
  }
}