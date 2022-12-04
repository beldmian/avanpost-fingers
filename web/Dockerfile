FROM node:18.9-alpine3.15 AS builder
WORKDIR /app
COPY package.json .
COPY package-lock.json .
RUN npm i
COPY . .
RUN npm run build

FROM nginx
COPY --from=builder /app/dist /usr/share/nginx/html

