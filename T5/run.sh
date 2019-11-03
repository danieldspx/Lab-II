export LC_ALL=C
test -d ./bin || mkdir ./bin
(make 2>&1 > /dev/null && ./bin/main) || echo "🙁 Erro ao executar\n"
