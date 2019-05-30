# Antarctica
Antarctica is an open-source project aiming to perform comfortable in-cloud backup of user files, installed packages list and dotfiles.

Current edition is a port of archived [Kotlin version](https://github.com/mad-penguins/AntarcticaKt) to C++/Qt.
It's being ported because of hardness of deployment an the most Linux installations.
Mostly it's caused by unavailability of Java SE 8 or OpenFX for Java 9 or newer.

Features (work still in progress):
- [ ] Files management
    - [ ] Uploading into server
    - [ ] Downloading from server
    - [ ] Deleting from disk
    - [ ] Deleting from server
- [ ] Packages management (zypper only now)
    - [ ] Uploading into server (by one, not full list)
    - [ ] Binding of package and dotfiles
    - [ ] Installing into system
    - [ ] Removing from system
    - [ ] Removing from server
- [ ] Repositories management (zypper only now)
    - [ ] Reading list of added repositories
    - [ ] Adding into system
    - [ ] Removing from system
- [ ] User interface
    - [ ] Files management tab
    - [ ] Packages management tab
    - [ ] Repositories management tab
    - [ ] Settings
    - [ ] Custom design
 - [ ] Under the hood
    - [ ] Security
        - [ ] Login and registration through HTTPS
        - [ ] Database connection through TLS

Roadmap ~~can~~ will be extended in the future.

Antarctica server is at the moment under development too. Code of server is closed and it's now unstable.
Public remote server is being tested now. Open API will be ~~opened~~ documented in the future. There's already present an early preview of [Qt API wrapper](https://github.com/mad-penguins/IcebreakerQt).

You can help us to rent a server and also support the development:
- WebMoney: R710781308549
- [Yandex Money](https://money.yandex.ru/to/410015281707280)

Any suggestions and contributions are welcome. Let's make Linux much more user friendly!
