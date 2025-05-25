FROM my-base-image

WORKDIR /app

COPY . .

RUN g++ -std=c++17 main.cpp -o my_program -pthread -lcpr -lpqxx -lpq -lcrypto

CMD ["./my_program"]
