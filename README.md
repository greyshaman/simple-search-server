<img src="https://github.com/greyshaman/simple-search-server/raw/refs/heads/master/images/orig%20-%202024-09-26T112502.725.webp" width="50%">

# Simple Search Server

Простой поисковый движок с базовой функциональностью индексирования текстовых файлов для последующего поиска. Он представляет собой консольное приложение, работа которого настраивается при помощи конфигурационного файла [config.json](#конфигурация), а поисковые запросы предоставляются при помощи файла [requests.json](#поисковые-запросы). После индексации файлов с текстовой информацией и считывания поисковых запросов, движок генерирует [answers.json](#результаты-поиска) с соответствующими ответами.

Этот проект может быть использован для дальнейшего встраивания в поисковый сервер, работающий в интерактивном режиме, например, на веб или через любой удобный интерфейс взаимодействия. Вся функциональная часть реализована в виде библиотеки, которую можно включить в другом проекте. Также имеется простейшая запускаемая консольная оболочка для демонстрации работы с библиотекой. 

## Описание 

Идеей создания этого проекта послужило учебное задание создать поисковый движок на с++. Собственно проект и есть результат выполнения этого интересного задания.

### Конфигурация

Для настройки поискового движка предлагается использовать файл _config.json_. Без этого файла конфигурации приложение не сможет работать. Поэтому, если файл _config.json_ не окажется в том же каталоге, что и исполняемый файл приложения, оно завершит свою работу и выдаст ошибку.

Пример конфигурационного файла _config.json_:

```json
{
  "config": {
    "name": "SearchServerEngine",
    "version": "0.1",
    "max_responses": 5
  },
  "files": [
    "./file001.txt",
    "./file002.txt",
    "./file003.txt",
    "./file004.txt"
  ]
}
```

- В секции ```config``` применяются следующие параметры:
  - ```name``` служит для указания названия приложения;
  - ```version``` указывает для какой версии поискового движка этот файл. Если версии не совпадут, то приложение выдаст ошибку;
  - ```max_responses``` устанавливает количество ранжированных ответов для каждого запроса. Этот параметр удобен для отсеивания несущественных результатов.
- В секции ```files``` перечисляются файлы с текстом, по которым будет производиться поиск.

Приложение автоматически обходит указанные текстовые документы и строит поисковый индекс.

Настоящая версия поискового движка рассчитана на обработку текстов, со словами состоящих из латинских букв, разделённых одним или несколькими пробелами. Каждый документ может содержать не более **1000** слов с максимальной длиной каждого слова не более **100** символов.  

### Поисковые запросы

Поисковые запросы представляют собой набор слов, разделённых пробелами, для которых нужно найти наиболее вероятные текстовые документы, в которых они встречаются. Запросы должны перечисляться в виде списка в файле _requests.json_.

Пример _requests.json_:

```json
{
  "requests": [
	  "milk water",
	  "sugar"
  ]
}
```

В настоящей версии поискового движка можно задать не более **1000** запросов и каждый запрос может включать в себя не более **10** слов.

### Результаты поиска

Сопоставив поисковые запросы с созданным индексом вхождения слов в текстовые документы, поисковый движок создаёт файл _answers.json_ с результатами, показывающими, насколько часто слова из запроса встречаются в каждом исходном текстовом документе.

Пример _answers.json_:

```json
{
    "answers": {
        "request001": {
            "relevance": [
                {
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 0,
                    "rank": 0.699999988079071
                },
                {
                    "docid": 1,
                    "rank": 0.30000001192092896
                }
            ],
            "result": "true"
        },
        "request002": {
            "result": "false"
        }
    }
}
```

Где ```answers``` содержит список пронумерованных первоначальных запросов. Секции запросов типа ```requestNNN``` могут содержать различные результаты:
- Если слова из запроса не встречаются ни в одном текстовом документе, то единственным полем в нём будет ```"result": "false"```
- Если слова из запроса встречаются только в одном документе, то секция запросы будет выглядеть следующим образом::

  ```json
  "request003": {
  	"docid": 3,
  	"rank": 1.0,
  	"result": "true"
  }
  ```
  
- Если слова из запроса встречаются в нескольких файлах, то секция с результатами для запроса будет выглядеть примерно так:

  ```json
  "request001": {
  	"relevance": [
  		{
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 0,
                    "rank": 0.699999988079071
                },
                {
                    "docid": 1,
                    "rank": 0.30000001192092896
                }
            ],
  	"result": "true"
  },
  ```

```doc_id``` - это номер текстового документа в списке ```files``` из [config.json](#конфигурация). ```rank``` - это относительная релевантность, показывающая, насколько часто слова из запроса встречаются в данном текстовом документе.

## Зависимости

Для реализации проекта применялись:
- Язык программирования С++17;
- Для создания тест кейсов - QtTest (Qt 5.15.2 или новее);
- Для запуска тестов использовалась QtCreator, но так как IDE запускает тесты при помощи ctest, то они могут запускаться ctest из коммандной строки;
- Для управления сборкой компонентов проекта - CMake 3.20 или новее
- Сборка проекта - make,GNU C++ compiler (g++13 или новее)

Разработка и проверка работоспособности велась под OS Linux.

## Как использовать

Данное приложение можно использовать двумя способами как библиотеку в составе другого проекта и как независимое приложение для демонстрации возможностей поискового движка.

### Запуск автономного приложения

**Этот пункт подлежит изменению**

Далее будут даны инструкции для сборки и запуска приложения под OS Linux, в которой подразумеваются установленными нужных [зависимостей](#зависимости)

После клонирования проекта в каталог для установки исходного кода:

```
git clone git@github.com:greyshaman/simple-search-server.git
```

Перейти в каталог simple-search-server:

```
cd simple-search-server
```

Создать каталог build и потом перейти в него:

```
mkdir build
cd build
```

Для демонстрации работы поискового движка он будет собран и установлен в каталог demo.
Сгенерировать конфигурацию для сборки проекта (так как для демонстрации не подразумевается установка в системные каталоги, то нужно изменить CMAKE_INSTALL_PREFIX):

```
cmake -DCMAKE_INSTALL_PREFIX=. ..
```

Запускаем сборку и инсталляцию:

```
make install
```

Если всё собралось и установилось, то переходим в каталог demo:

```
cd demo
```

В каталоге должны быть исполняемый файл который запускает демонстрацию **search-server**, библиотека **libSearchServerLib.so** и файлы конфигурации **config.json**, запросов **requests.json** и текстовые документы **file001.txt**, **file002.txt**, **file003.txt**, **file004.txt** по которым будет производится поиск.
Запускаем приложение:

```
./search-server
```

После выполнения программы:

```
Starting SearchServerEngine version 0.1
Texts are indexed
Reading requests
Search results complete and will write into result file
Answers wrote into file
Program complete. Thank you.
Bye
```

в каталоге demo появится **answers.json**

Далее изменяя запросы или тексты можно запустить повторно и поэкспериментировать с поисковым движком.

### Использование библиотеки

... Нужно дописать про библиотеку... 
