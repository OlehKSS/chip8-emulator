#include <print>
#include <QApplication>
#include <QImage>
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QThread>

#include "chip8.h"


class EmulationThread : public QThread 
{
	Q_OBJECT

public:
	EmulationThread(Chip8 *chip8, QObject *parent = nullptr)
		: QThread(parent)
		, chip8(chip8) 
	{
		
	}

protected:
	void run() override
	{
		std::println("Starting emulation thread...");
		while (!isInterruptionRequested()) 
		{
			chip8->emulateCycle();
			if (chip8->drawFlag) 
			{
				emit updateScreen();
				chip8->drawFlag = false;
			}
			// Delay to simulate the speed of a typical CHIP-8 processor
			// Othewise keyboard release event is too slow
			QThread::msleep(1);
		}
	}

signals:
	void updateScreen();

private:
	Chip8 *chip8;
};

class Chip8Widget : public QWidget
{
	Q_OBJECT

public:
	Chip8Widget(QWidget *parent = nullptr)
		: QWidget(parent) 
	{
		setFixedSize(Chip8::SCREEN_WIDTH * modifier, Chip8::SCREEN_HEIGHT * modifier);
		screen = QImage(Chip8::SCREEN_WIDTH, Chip8::SCREEN_HEIGHT,  QImage::Format_RGB32);
		emulationThread = new EmulationThread(&myChip8, this);
		timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &Chip8Widget::setUpdateTimers);
		timer->start(16); // roughly 60Hz

		connect(emulationThread, &EmulationThread::updateScreen, this, &Chip8Widget::updateScreen);
	}

	void loadGame(const char *filename) 
	{
		myChip8.loadGame(filename);
	}

protected:
    void showEvent(QShowEvent *event) override 
	{
        QWidget::showEvent(event); // Call base class implementation
		this->emulationThread->start();
    }

	void closeEvent(QCloseEvent *event) override 
	{
		emulationThread->requestInterruption();
        emulationThread->quit();
        emulationThread->wait();
        event->accept(); // Accept the close event
    }

	void paintEvent(QPaintEvent *event) override 
	{
		QPainter painter(this);
		for (int y = 0; y < Chip8::SCREEN_HEIGHT; ++y) 
		{
			for (int x = 0; x < Chip8::SCREEN_WIDTH; ++x) 
			{
				if (myChip8.gfx[(y * Chip8::SCREEN_WIDTH) + x] == 0) 
				{
					screen.setPixel(x, y, qRgb(0, 0, 0));
				} 
				else 
				{
					screen.setPixel(x, y, qRgb(0, 255, 0));
				}
			}
		}
		painter.drawImage(0, 0, screen.scaled(Chip8::SCREEN_WIDTH * modifier, Chip8::SCREEN_HEIGHT * modifier));
	}

	void keyPressEvent(QKeyEvent *event) override 
	{
		switch (event->key()) 
		{
			case Qt::Key_Escape: QApplication::quit(); break;
			case Qt::Key_1: myChip8.key[0x1] = 1; break;
			case Qt::Key_2: myChip8.key[0x2] = 1; break;
			case Qt::Key_3: myChip8.key[0x3] = 1; break;
			case Qt::Key_4: myChip8.key[0xC] = 1; break;
			case Qt::Key_Q: myChip8.key[0x4] = 1; break;
			case Qt::Key_W: myChip8.key[0x5] = 1; break;
			case Qt::Key_E: myChip8.key[0x6] = 1; break;
			case Qt::Key_R: myChip8.key[0xD] = 1; break;
			case Qt::Key_A: myChip8.key[0x7] = 1; break;
			case Qt::Key_S: myChip8.key[0x8] = 1; break;
			case Qt::Key_D: myChip8.key[0x9] = 1; break;
			case Qt::Key_F: myChip8.key[0xE] = 1; break;
			case Qt::Key_Z: myChip8.key[0xA] = 1; break;
			case Qt::Key_X: myChip8.key[0x0] = 1; break;
			case Qt::Key_C: myChip8.key[0xB] = 1; break;
			case Qt::Key_V: myChip8.key[0xF] = 1; break;
		}
	}

	void keyReleaseEvent(QKeyEvent *event) override 
	{
		switch (event->key()) 
		{
			case Qt::Key_1: myChip8.key[0x1] = 0; break;
			case Qt::Key_2: myChip8.key[0x2] = 0; break;
			case Qt::Key_3: myChip8.key[0x3] = 0; break;
			case Qt::Key_4: myChip8.key[0xC] = 0; break;
			case Qt::Key_Q: myChip8.key[0x4] = 0; break;
			case Qt::Key_W: myChip8.key[0x5] = 0; break;
			case Qt::Key_E: myChip8.key[0x6] = 0; break;
			case Qt::Key_R: myChip8.key[0xD] = 0; break;
			case Qt::Key_A: myChip8.key[0x7] = 0; break;
			case Qt::Key_S: myChip8.key[0x8] = 0; break;
			case Qt::Key_D: myChip8.key[0x9] = 0; break;
			case Qt::Key_F: myChip8.key[0xE] = 0; break;
			case Qt::Key_Z: myChip8.key[0xA] = 0; break;
			case Qt::Key_X: myChip8.key[0x0] = 0; break;
			case Qt::Key_C: myChip8.key[0xB] = 0; break;
			case Qt::Key_V: myChip8.key[0xF] = 0; break;
		}
	}

private slots:
	void updateScreen()
	{
		update();
	}

	void setUpdateTimers()
	{
		myChip8.setUpdateTimers();
	}

private:
	Chip8 myChip8;
	QImage screen;
	EmulationThread *emulationThread;
	QTimer *timer;
	int modifier = 15;
};

int main(int argc, char *argv[]) 
{
	QApplication app(argc, argv);

	if (argc < 2) 
	{
		std::println("Usage: chip8emu <chip8game.c8>");
		return 1;
	}

	Chip8Widget widget;
	widget.loadGame(argv[1]);
	widget.show();

	return app.exec();
}

#include "main.moc"
