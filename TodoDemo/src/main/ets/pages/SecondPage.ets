import router from '@ohos.router'

@Entry
@Component
struct SecondPage {
  @State message: string = 'Second Page';
  @State src: string = (router.getParams() as Record<string, string>)['src'];

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize('38fp')
          .fontWeight(FontWeight.Bold)
        Text(this.src)
          .fontSize('20fp')
          .opacity(0.6)
        Blank()
        Button('Back')
          .fontSize('16fp')
          .width('296vp')
          .height('40vp')
          .backgroundColor('#007DFF')
          .onClick(() => {
            console.info(`Succeeded in clicking the 'Back' button.`)
            try {
              // 返回第一页
              router.back()
              console.info('Succeeded in returning to the first page.')
            } catch (err) {
              console.error(`Failed to return to the first page.Code is ${err}`)
            }
          })

        LoadingProgress()
          .color(Color.Blue)
          .height(60)
          .width(60)
      }
      .width('100%')
    }
    .height('100%')
  }
}