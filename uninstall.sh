#!/bin/bash

# Подключаем конфигурацию установки
source /etc/bz_conf/install.conf

if [ "$1" == "1" ]; then
    # Удаляем временную директорию
    echo "Удаление временной директории: $TEMP_PATH"
    rm -rf "$TEMP_PATH"

    # Удаляем директорию сертификата
    echo "Удаление директории сертификата: $CERTIFICATE_PATH"
    rm -rf "$CERTIFICATE_PATH"
fi

if [ "$1" == "2" ]; then
    # Удаляем временную директорию
    echo "Удаление временной директории: $TEMP_PATH"
    rm -rf "$TEMP_PATH"

    # Удаляем директорию сертификата
    echo "Удаление директории сертификата: $CERTIFICATE_PATH"
    rm -rf "$CERTIFICATE_PATH"

    # Удаляем директорию сохранений
    echo "Удаление директории сохранений: $SAVE_PATH"
    rm -rf "$SAVE_PATH"
fi

echo "Деинсталляция завершена успешно"

