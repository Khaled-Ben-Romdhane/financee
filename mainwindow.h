#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "finance.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        finance f;

private slots:
    // Slots for each button click
    void on_projet_clicked();
    void on_employer_clicked();
    void on_materielle_clicked();
    void on_fournisseur_clicked();
    void on_facture_clicked();
  void  on_pushButton_ajouter_clicked();
  void  on_pushButton_supprimer_clicked();
  void  on_pushButton_modifierer_clicked();
  void on_pushButton_modifier_f_clicked();
  void on_ACTUALISER_clicked();
  void on_comboBox_trii_changed(int index);
  void on_lineEdit_15_cursorPositionChanged(const QString& text);
  void on_pushButton_9_clicked();
 // void on_PDF_clicked();




private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
