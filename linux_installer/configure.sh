#!/bin/bash

# Спрашиваем у пользователя выбор пути установки
read -p "Хотите использовать стандартный путь установки (/home/vboxuser/bz)? (y/n): " USE_DEFAULT

if [ "$USE_DEFAULT" == "y" ]; then
    INSTALL_PATH="/home/vboxuser/bz"
else
    read -p "Введите путь для установки (будут созданы подкаталоги): " INSTALL_PATH
fi

# Создаем директории, если они не существуют
CERTIFICATE_PATH="$INSTALL_PATH/certificates"
TEMP_PATH="$INSTALL_PATH/temp"
SAVE_PATH="$INSTALL_PATH/saves"

mkdir -p "$CERTIFICATE_PATH"
mkdir -p "$TEMP_PATH"
mkdir -p "$SAVE_PATH"

# Записываем пути в install.conf
echo "TEMP_PATH=$TEMP_PATH" > "install.conf"
echo "SAVE_PATH=$SAVE_PATH" >> "install.conf"
echo "CERTIFICATE_PATH=$CERTIFICATE_PATH" >> "install.conf"

# Спрашиваем у пользователя данные
read -p "Введите ваше ФИО: " FULL_NAME
read -p "Введите ваш E-mail: " EMAIL
read -p "Введите ваш номер телефона: " PHONE_NUMBER

# Формируем сертификат
CERTIFICATE_FILE="$CERTIFICATE_PATH/certificate.txt"
echo "Сертификат на свободное использование ВС" > "$CERTIFICATE_FILE"
echo "Пользователь: $FULL_NAME" >> "$CERTIFICATE_FILE"
echo "E-mail: $EMAIL" >> "$CERTIFICATE_FILE"
echo "Номер телефона: $PHONE_NUMBER" >> "$CERTIFICATE_FILE"
echo "Лицензия распространения: GNU General Public License" >> "$CERTIFICATE_FILE"
echo "Дата установки: $(date)" >> "$CERTIFICATE_FILE"

echo "Сертификат успешно создан и сохранен в $CERTIFICATE_FILE"

