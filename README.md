# soica_test
Проект разделён на две части:  
* python - код запуска HINet и конвертации моделей в onnx
* cpp - код двух приложений для использования сконвертированной onnx модели: с консольным интерфейсом и графическим  
## git clone
Репозиторий HINet включён как сабмодуль в python, поэтому для корректной работы необходимо выполнить следующие команды:
```
git clone https://github.com/MrLIk/soica_test
cd soica_test
git submodule update --init
```