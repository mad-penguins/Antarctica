# Antarctica
![screenshot of sample](screenshots/mainscreen.png)

Antarctica is an open-source project aiming to perform comfortable in-cloud backup of user files, installed packages list and dotfiles.

Current edition is a port of archived [Kotlin version](https://github.com/mad-penguins/AntarcticaKt) to C++/Qt.
It's being ported because of hardness of deployment an the most Linux installations.
Mostly it's caused by unavailability of Java SE 8 or OpenFX for Java 9 or newer.

Features (work still in progress):
- [ ] Files management
    - [x] Uploading into server
    - [x] Downloading from server
    - [ ] Deleting from disk
    - [x] Deleting from server
- [ ] Packages management (zypper only now)
    - [x] Uploading into server (by one, not full list)
    - [x] Binding of package and dotfiles
    - [ ] Installing into system
    - [ ] Removing from system
    - [x] Removing from server
- [ ] Repositories management (zypper only now)
    - [ ] Reading list of added repositories
    - [ ] Adding into system
    - [x] Removing from system
- [ ] User interface
    - [x] Files management tab
    - [x] Packages management tab
    - [ ] Repositories management tab
    - [ ] Settings
    - [ ] Custom design
 - [ ] Under the hood
    - [x] Security
        - [x] Connection through HTTPS
    - [ ] Real-time files' states changes monitoring

Roadmap ~~can~~ will be extended in the future.

Antarctica server is at the moment under development too. Code of server is closed and it's now unstable.
Public remote server is being tested now. Open API will be ~~opened~~ documented in the future. There's already present an early preview of [Qt API wrapper](https://github.com/mad-penguins/IcebreakerQt).

You can help us to rent a server and also support the development:
- WebMoney: R710781308549
- [Yandex Money](https://money.yandex.ru/to/410015281707280)

Any suggestions and contributions are welcome. Let's make Linux much more user friendly!
