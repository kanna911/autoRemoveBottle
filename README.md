# autoRemoveBottle
Low-cost Arduino-based automatic water restriction for animal cages

Water restriction is an important variable that must be carefully controlled in animal experiments. In this study, we developed a low-cost device that automatically lifts a water bottle at a specified time, aiming to improve experimental efficiency and reduce labor. By using an Arduino microcontroller in combination with a relay module and an RTC (real-time clock) module, we implemented a mechanism to activate a linear actuator and lift the water bottle on schedule. To stabilize the bottle's position, we also designed a custom support structure using a 3D printer.

<img width="1117" alt="Image" src="https://github.com/user-attachments/assets/2efa687e-0d86-481c-af1e-e109c9997bed" />

![Image](https://github.com/user-attachments/assets/283dede5-81be-4fbb-9d3b-4735c37dd2e6)




### autoRemoveBottle.ino (arduino IDE code)
→ 現在作成中

タイマー処理（行X〜行X）：
Arduinoのloop()関数内では、定期的（例：毎秒）に現在時刻を取得し、あらかじめ設定された動作予定時刻と比較する処理が行われる。時刻が一致した場合にのみ、リレーまたはモータードライバを介してリニアアクチュエーターを動作させるように設計されている。

スケジューリング処理（行X〜行X）：
動作時刻はRTCモジュールの出力と照合される。毎週特定の曜日・時刻や毎日決まった時間に動作させるために、現在の曜日や時刻を条件として評価し、次回の動作予定時刻をDateTimeオブジェクトとして記録・比較することで制御が可能となっている。また、LCDディスプレイを併用することで、現在時刻および次回の動作予定時刻を視覚的に表示でき、ユーザーによる確認や管理が容易になる。

例として、(1)毎週日曜日の正午（12:00）、(2)毎日正午（12:00）、(3)任意の指定時刻、にタイマーをセットする関数を予め記載している。
(1) 毎週日曜日の正午（12:00）にタイマーを設定するには、
```
nextRun = getNextSundayNoon();
```

(2) 毎日正午（12:00）にタイマーを設定するには、
```
nextRun = getNextDailyNoon();
```

(3) 任意の指定時刻にタイマーを設定するには、
```
nextRun = DateTime(YYYY, MM, DD, HH, MM, SS);
```

上記のコードをコメントアウト（//を外す）して活用して頂きたい。
(3)のコードについては、YYYYに西暦を、MMに月を、DDに日を、HHに時間を、MMに分を、SSに秒を入力する。例えば、2025年6月1日 15:30:00 に実行したい場合は、
```
nextRun = DateTime(2025, 6, 1, 15, 30, 00);
```
と入力する。

