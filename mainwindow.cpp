#include "mainwindow.h"
#include "ui_mainwindow.h"

auto t1 = std::chrono::high_resolution_clock::now(), t2 = std::chrono::high_resolution_clock::now();
int num_clicks = 1;
bool first_round = true;
bool has_started = false;
QString cps_text;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Start a new timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(counter()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::counter()
{
    t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1);

    // When timer is finished
    if(duration.count() > 5000){
        ui->timer_label->setText("Timer: 0.0");
        first_round = false;
        num_clicks = 1;
        has_started = false;
        ui->box->setText("Click to Start");
        return;
    }

    // Update the time
    double time = std::round((5000-duration.count())/100)/10.0;
    QString time_text = "Timer: " + QString::number(time);
    ui->timer_label->setText(time_text);

    // Update clicks per second
    double cps = std::floor(num_clicks / (5.0-time)*10)/10.0;
    cps_text = QString::number(cps);
    ui->cps_label->setText(" CPS: " + cps_text + "  ");

}


void MainWindow::on_box_clicked()
{
    if(has_started){
        // Count the number of clicks
        num_clicks++;
    }else{
        if(!first_round){
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText("\nYou got " + cps_text + " clicks per second.\nOnce you press the OK button the click test will begin again.");
            msg.setWindowTitle("Congratulations!");
            msg.exec();
        }
        // Reset the timer
        t1 = std::chrono::high_resolution_clock::now();
        timer->start(100);
        has_started = true;
    }
    // Display current score
    QString clicks_text = " Score: " + QString::number(num_clicks) + " ";
    ui->score_label->setText(clicks_text);
    ui->box->setText("");
}
