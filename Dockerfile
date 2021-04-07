FROM node:14.15

RUN echo "VERSION 1";

WORKDIR /opt/app

COPY yarn.lock .
COPY package.json .

RUN yarn install

CMD ["sh", "-c", "cp -r node_modules code; cd code; tail -f /dev/null"]

