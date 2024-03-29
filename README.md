# Установка и обновление

## Что нужно скачать

1. [git](https://git-scm.com/);
2. [cmake](https://cmake.org/download/) - для компиляции wxWidgets;
3. [wxWidgets](https://www.wxwidgets.org/downloads/) - zip или 7z архив.

## Компиляция wxWidgets

Открываем консоль (PowerShell, CMD, gitbash или что-то еще) в распакованном архиве и вводим следующие команды

> На всякий случай напишу. Быстро открыть PowerShell можно, нажав правой кнопкой мыши с зажатым шифтом и выбрав `Открыть окно PowerShell здесь`. Важно чтобы в проводнике не было выбрано файлов, а то подсказки будут для файла, а не каталога.

```powershell
PS> cmake . -DwxBUILD_SHARED=OFF
PS> cmake --build . --target install
```

> По умолчанию скомпилированные исходники будут находится в папке C:\Project Files(x86)\wxWidgets.

> Если есть желание, то можно изменить эту папку добавив флаг `-DCMAKE_INSTALL_PREFIX=<Путь к папке>`.

То есть, например следюущие команды скомпилируют библиотеку в папку wxWidgets  на диске C.

```powershell
PS> cmake . -DCMAKE_INSTALL_PREFIX=C:\wxWidgets -DwxBUILD_SHARED=OFF
PS> cmake --build . --target install
```

После компиляции нужно создать переменную среды с путем к исходникам. Для этого вводим в поиск винды `Изменение системных переменных среды`. В открышемся окне выбираем `Переменные среды` и создаем переменную с именем `wxPath` и значением с путем к папке с исходниками wxWidgets.

## Загрузка проекта

Открываем консоль в папке где должен находится проект. Вводим следующие команды.

```powershell
PS> git clone https://github.com/mk101/text-editor-cpp.git
PS> cd text-editor-cpp
PS> ii .
```

Откроется папка с проектом. Открыаем `.sln` файл и пробуем запустить. По идее должно все заработать.

Далее стоит создать свою ветку, в которой нужно будет все писать. Для этого вводим следующие команды:

```powershell
PS> git branch <название>
PS> git checkout <название>
```

В качестве названия можно использовать имя или фамилию, но если хочется можно написать все что угодно.

## Обновление репозитория

> Я пока не проверял merge и pull, так что могу ошибаться. Но вроде бы должно работать.

Если кто-то запушил свой код на гитхаб, то нужно обновить свой локальный репозиторий. Для этого стоит ввести следующие команды:

```powershell
PS> git checkout main
PS> git pull
PS> git checkout <название ветки>
PS> git merge main
```

После написания какой-то части кода стоит его сохранить. Для этого нужны следующие команды:

```powershell
PS> git add .
PS> git commit -m "<Имя коммита>"
```

> Эти команды должны вводиться в директории text-editor-cpp.

Для того, чтобы загрузить все эти изменения на github, вводим следующие команды:

```powershell
PS> git push -u origin <название ветки>
```

> Чтобы узнать имя текущей ветки можно ввести `git status`. Или ввести `git branch -a`, чтобы увидеть все ветки.

После удачного обновления стоит создать Pull Request на гитхабе.

## Содержание решения

Решение содержит 3 проекта:

- TextEditor - GUI
- Core - Код
- CoreTest - Тесты

> Все изначальные файлы были созданы для проверки работы линкера и их можно удалить.
