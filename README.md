# Náplň cvičenia
- zoznámenie sa s I2C
- komunikácia s IMU prostredníctvom I2C


# I2C zbernica

### Základné posielanie dát
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_9/blob/master/docs/I2C_Basic_Address_and_Data_Frames.jpg" width="950" title="">
</p>

### Posielanie večšieho množstva dát
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_9/blob/master/docs/I2C_Repeated_Start_Conditions.jpg" width="550" title="">
</p>

# Konfigurácia I2C

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_9/blob/master/images/i2c_conf.PNG" width="850" title="GPIO pin block scheme">
</p>

- na obrázku je zobrazená konfigurácia I2C - okrem zobrazených parametrov je nutné povoliť globálne prerušenia od I2C ("error interrupt" nie je momentálne potrebný)
- MCU má rolu "master", číta z registrov alebo zapisuje do registrov senzora ("slave") 


# IKS01A1 senzorová doska

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_9/blob/master/images/sensor_board.jpg" width="350" title="GPIO pin block scheme">
</p>

- obsahuje viacero senzorových jednotiek: magnetometer, IMU, sonzor vlhkosti ...
- v tomto prípade MCU komunikuje s IMU (acc a gyro) LSM6DS0
- na obrázku je znázornené zapojenie senzorovej dosky k vývojovej doske s MCU

- ukážkový program komunikuje s LSM6DS0 prostredníctvom I2C a číta hodnotu zrýchlení v osiach x, y, z

# Zadanie
- WORK IN PROGRESS !!!

