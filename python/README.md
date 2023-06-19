# python
Перед началом использования скриптов необходимо загрузить обученные веса. Ссылки на них доступны в оригинальном репозитории и продублированны здесь:
* [HINet-SIDD-0.5x](https://drive.google.com/file/d/1Y5YJQVNL0weifE--5us344bLwzBNS_sU/view?usp=sharing)
* [HINet-SIDD-1x](https://drive.google.com/file/d/1CU5z-M90Jc-TAcVpEaFjDCYA09fkubGi/view?usp=sharing)
* [HINet-GoPro](https://drive.google.com/file/d/1dw8PKVkLfISzNtUu3gqGh83NBO83ZQ5n/view?usp=sharing)
* [HINet-REDS](https://drive.google.com/file/d/1uYH8XvLgrn-Vg6L0NjUcO2Fblhqrc8TU/view?usp=sharing)
* [HINet-Rain13k](https://drive.google.com/file/d/1AVedAkb1B2F2b3XGWlMFFVSsNfQlCwxa/view?usp=sharing)
## Описание содержимого
* *HINet/* - директория-сабмодуль оригинального репозитория
* *requirements.txt* - зависимоти python, содержат ссылку на requirements HINet
* *infer_like_demo.py* - скрипт, который воспроизводит инференс по образцу исходного репозитория
* *infer_like_demo.yml* - файл конфигурации запуска для скрипта *infer_like_demo.py*, основанный на файле конфигураций из оригинального репозитория
* *convert_to_onnx.py* - скрипт, который производит конвертацию из pth в onnx
* *convert_to_onnx.yml* - файл конфигурации запуска для скрипта *convert_to_onnx.py*, основанный на файле конфигураций из оригинального репозитория
* *infer_onnx.py* - скрипт инференса модели в onnx формате
* *compare_2_images.py* - вспомогательный скрипт сравнения двух изображений методом суммирования модуля разности всех пикселей
## Настройка окружения на Linux
Перед началом работы необходимо настроить окружение. Здесь приведена последователность шагов для настройки venv. Подразумевается, что все шаги выполняются из директории **soica_test/python**.
1. Создать виртуальную среду
```
python3 -m venv env
```
2. Активировать её
```
source ./env/bin/activate
```
3. Установить необходимые зависимости
```
pip install -r requirements.txt
```
4. Выполнить export PYTHONPATH
```
export PYTHONPATH=$PYTHONPATH:$(pwd)/HINet
```
## Использование *infer_like_demo.py*
1. Настроить файл конфигурации, основные параметры
* num_gpu - количество gpu для запуска (0 - cpu запуск)
* input_img - путь до исходного изображения
* output_img - путь, куда сохранить полученное изображение
* блок network_g - необходимо задать верные параметры, соответствующие обученным весам, их можно посмотреть в директории оригинального репозитория *HINet/options/train*
* pretrain_network_g - путь до обученных весов
* grids - использовать ли разбиение изображения на блоки
2. Запустить скрипт
```
python ./infer_like_demo.py -opt ./infer_like_demo.yml
```
## Использование *convert_to_onnx.py*
Полученная модель будет имеет onnx opset=7 и нефиксированные размерности N H W. Единственная фиксированная размерность - C.  
**ВНИМАНИЕ!** Рекомендуется использовать полученную модель для изображений, стороны которых равны 256, т.к. именно в таком режиме обучались исходные сети. Хотя тесты показали, что использование в качестве входных данных изображения размера 1280x720 не приводит к ощутимому ухудшению работы.
1. Настроить файл конфигурации как в предыдущем пункте
2. Настроить дополнительную секцию *convert*
* onnx_path - путь до итогового onnx файлв
* input_w - ширина входного изображения
* input_h - высота входного изображения
* input_ch - количество каналов входного изображения
3. Запустить скрипт
```
python ./convert_to_onnx.py -opt ./convert_to_onnx.yml
```
##  Использование *infer_onnx.py*
Скрипт имеет следующие входные параметры
```
options:
  -h, --help            show this help message and exit
  --onnx_path ONNX_PATH
                        Path to the onnx model
  --input_image_path INPUT_IMAGE_PATH
                        Input image path
  --output_image_path OUTPUT_IMAGE_PATH
                        Output image path
```
Пример запуска скрипта
```
python ./infer_onnx.py --onnx_path HINet-GoPro.onnx --input_image_path ./HINet/demo/GOPR0384_11_00-000001.png --output_image_path ./res.png
```
## Использование *compare_2_images.py*
Скрипт имеет следующие входные параметры
```
options:
  -h, --help            show this help message and exit
  --first_image_path FIRST_IMAGE_PATH
  --second_image_path SECOND_IMAGE_PATH
```
Пример запуска скрипта
```
python ./compare_2_images.py --first_image_path ./demo1.png --second_image_path ./res.png
```
## Возможные улучшения
1. Перейти на использование docker вместо venv
2. Добавить шаг оптимизации архитектуры сети при конвертации в onnx