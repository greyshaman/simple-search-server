<img src="https://github.com/greyshaman/simple-search-server/raw/refs/heads/master/images/orig%20-%202024-09-26T112502.725.webp" width="50%">

# Simple Search Server

Простой поисковый движок с базовой функциональностью индексирования текстовых файлов для последующего поиска. Он представляет собой консольное приложение, работа которого настраивается при помощи конфигурационного файла config.json, а поисковые запросы предоставляются при помощи файла requests.json. После индексации файлов с текстовой информацией и считывания поисковых запросов, движок генерирует answers.json с соответствующими ответами.

Этот проект может быть использован для дальнейшего встраивания в поисковый сервер, работающий в интерактивном режиме, например, на веб или через любой удобный интерфейс взаимодействия.

## Описание 

Идеей создания этого проекта послужило учебное задание создать поисковый движок на с++. Собственно проект и есть результат выполнения этого интересного задания.

### Конфигурация

Для настройки поискового движка предлагается использовать файл config.json. Без этого файла конфигурации приложение не сможет работать. Поэтому, если файл config.json не окажется в том же каталоге, что и исполняемый файл приложения, оно завершит свою работу и выдаст ошибку.

Пример конфиурационного файла config.json:

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
