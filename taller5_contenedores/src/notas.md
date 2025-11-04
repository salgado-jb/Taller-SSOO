### Dockerfile:
* RUN corre al compilar
* CMD corre al ejecutar
* el servidor externo tiene que exponer el puerto 8080 para que el local se pueda conectar (el cliente se conecta con el local mediante sockets unix)


### docker-compose.yaml:
* `environment` acá defino la variable de entorno EXTERNAL_HOST que después uso en localServer.c (`const char *host = getenv("EXTERNAL_HOST");`)
    * la variable va a tener el valor literal "external"
    * en `getaddrinfo("external", "8080", ...)` el SO del contenedor le pregunta al DNS de Docker "¿Quién es external?" y este responde con la IP local.
* `depends on` para que asegurarme que se ejecute external antes que local

### Para conectarme con el cliente:
* En una nueva terminal, ejecuto: `docker-compose exec local bash` para entrar al contenedor local
* ejecuto: `/app/localClient`
* ya puedo operar

### Para el ejercicio 2:
* Para que pueda conectarme al servidor externo corriendo en un contenedor desde un servidor interno corriendo en el host, tengo que mapear el puerto 8080 del contenedor al puerto 8080 del host.
    * lo agrego con `ports:`
* Corriendo el servidor local fuera del contenedor del ejercicio 1: `getenv` va a devolver NULL, y `getaddrinfo(NULL, "8080", ...)` va a devolver `localhost`