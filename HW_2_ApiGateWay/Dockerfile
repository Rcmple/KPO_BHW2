FROM my-base-image

WORKDIR /app

COPY . .

RUN g++ -std=c++17 main.cpp -o my_program -pthread -lcpr -lcurl -lssl -lcrypto -lpqxx -lpq

CMD ["./my_program"]
