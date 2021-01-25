TODO(jon) update README

```text
15:34:40.023 -> Starting PMIC test.. 1
15:34:40.023 -> enableCharge() : 1
15:34:40.023 -> getChargeCurrent() : 0.5120000243
15:34:40.023 -> getChargeFault() : 0
15:34:40.023 -> getChargeVoltage() : 4.1119999886
15:34:40.023 -> getInputCurrentLimit() : 3.0000000000
15:34:40.023 -> getInputVoltageLimit() : 3.8800001144
15:34:40.023 -> getMinimumSystemVoltage() : 3.5000000000
15:34:40.023 -> getPreChargeCurrent() : 0.2560000122
15:34:40.023 -> getTermChargeCurrent() : 0.2560000122
15:34:40.023 -> isBattConnected() : 0
15:34:40.023 -> isBatteryInOverVoltage() : 0
15:34:40.023 -> isHot() : 0
15:34:40.023 -> isPowerGood() : 1
15:34:40.023 -> isWatchdogExpired() : 0
15:34:40.023 -> canRunOnBattery() : 0
15:34:40.023 -> chargeStatus() : 0x20 FAST_CHARGING
15:34:40.023 -> Starting Modem, GSM and GPRS 357520078534019 Modem started!  GSM_READY  GPRS_READY
15:34:58.936 -> .TCP Connection to server is UP!, sending '{"STATUS":"RESTARTING"}'
15:35:01.423 -> HTTP/1.1 201 Created
15:35:01.423 -> Date: Fri, 22 Jan 2021 15:35:01 GMT
15:35:01.423 -> Connection: close
15:35:01.423 -> Content-Length: 0
15:35:01.423 ->
15:36:01.684 -> PMIC.chargeStatus() : 0x10 PRE_CHARGING
15:36:01.684 -> ADC_BATTERY : 3.0221896383
15:36:04.207 -> Sending update to server..
15:36:04.240 -> TCP Connection to server is UP!, sending '{"ADC_BATTERY":3.6737047898}'
15:37:01.648 -> PMIC.chargeStatus() : 0x20 FAST_CHARGING
15:37:01.648 -> ADC_BATTERY : 3.5391984360
15:37:02.875 -> Sending update to server..
15:37:02.908 -> TCP Connection to server is UP!, sending '{"ADC_BATTERY":3.3836754643}'
```