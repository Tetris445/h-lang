#!/bin/bash
echo "Проверяем интернет через h2c..."
echo 'net https://httpbin.org/get' > temp_net.h
./h2c temp_net.h | grep -q "origin" && echo "✅ Интернет есть!" || echo "❌ Нет соединения"
rm temp_net.h
