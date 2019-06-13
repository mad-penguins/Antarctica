/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <QApplication>
#include "ui/LoginWindow.h"
#include "ui/MainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    auto loginWindow = new LoginWindow(QApplication::arguments().contains("-local"));
    auto mainWindow = new MainWindow;
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(loginWindow, &LoginWindow::loggedIn, mainWindow, &MainWindow::showUser);

    loginWindow->show();

    return QApplication::exec();
}
