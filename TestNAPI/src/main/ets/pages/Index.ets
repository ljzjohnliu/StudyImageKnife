import hilog from '@ohos.hilog';
import testNapi from 'libtestnapi.so';

@Entry
@Component
struct Index {
  @State message: string = '调用Native的Add方法';
  @State message1: string = '调用Native的CallNative方法';
  @State message2: string = '调用Native的NativeCallArkTS方法';

  build() {
    Row() {
      Column() {
        // 第一个按钮，调用add方法，对应到Native侧的Add方法，进行两数相加。
        Text(this.message)
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            hilog.info(0x0000, 'testTag', 'Test NAPI 2 + 3 = %{public}d', testNapi.add(2, 3));
            this.message += "结果是：" + testNapi.add(2, 3);
          })
        // 第二个按钮，调用乘积方法，对应到Native侧的CallNative方法，进行两数相乘。
        Text(this.message1)
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            this.message1 += "结果是：" +  testNapi.callNative(2, 3);
          })
        // 第二个按钮，调用nativeCallArkTS方法，对应到Native的NativeCallArkTS，在Native调用ArkTS function。
        Text(this.message2)
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            this.message2 += testNapi.nativeCallArkTS((a: number) => {
              return a * 2;
            });
          })
      }
      .width('100%')
    }
    .height('100%')
  }
}
