#ifndef SMARTCARPENTRY_H
#define SMARTCARPENTRY_H

#include <QMainWindow>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QVector>

#include "employee.h"
#include "bois.h"
#include "fabrication.h"
#include "phase.h"
#include "modele.h"
#include "piece.h"
#include "modelecostestimator.h"
#include "nestingengine.h"
#include "nestingwidget.h"
#include "piecerect.h"
#include "panel.h"
#include "arduinomanager.h"
#include "arduinohandler.h"
#include "arduinoranim.h"

struct PieceNestingPlacement
{
    QString code;
    QString nom;
    int feuille = 0;
    int ordre = 0;
    double xMm = 0.0;
    double yMm = 0.0;
    double largeurMm = 0.0;
    double hauteurMm = 0.0;
    bool rotation = false;
    bool grainBloque = false;
};

class QChartView;
class QChart;
class QLineSeries;
class QDateTimeAxis;
class QValueAxis;
class QBarSeries;
class QBarSet;
class QTableWidget;
class QTableView;
class QLineEdit;
class QComboBox;
class QPushButton;
class QHeaderView;
class QLabel;
class QProgressBar;
class QTimer;
class QTextEdit;
class QFrame;
class QSpinBox;
class PieceFilterProxyModel;
class ModeleFilterProxyModel;
class QTabWidget;
class AIAssistantService;
class RfidManager;
class QTextBrowser;
class BudgetGltfViewerWidget;

enum class PieceDraftUiState
{
    None,
    Dirty,
    Saved,
    Published,
    NestingValidated,
    AtelierValidated
};

QT_BEGIN_NAMESPACE
namespace Ui {
class smartcarpentry;
}
QT_END_NAMESPACE

class smartcarpentry : public QMainWindow
{
    Q_OBJECT

public:
    smartcarpentry(const QString &currentUserName = QString(),
                   const QString &currentUserImagePath = QString(),
                   const QString &currentUserEmail = QString(),
                   QWidget *parent = nullptr);
    ~smartcarpentry();

private slots:
    // Modele CRUD slots
    void on_btn_mod_add_clicked();
    void on_btn_mod_edit_clicked();
    void on_btn_mod_del_clicked();
    void on_btn_mod_clr_clicked();

    // Modele table selection
    void on_tv_mod_clicked(const QModelIndex &index);

    // Modele search
    void on_search_mod_textChanged(const QString &text);

    // Modele stats
    void on_btn_mod_stats_refresh_clicked();

    // Modele advanced features
    void on_btn_mod_estimate_image_clicked();
    void on_btn_mod_budget_generate_clicked();
    void on_btn_mod_perf_calculate_clicked();
    void on_btn_mod_perf_export_pdf_clicked();

    // Modele PDF export
    void on_btn_mod_export_pdf_clicked();

    // Modele inline editing
    void on_tv_mod_cellChanged(int row, int column);

    // Employee CRUD slots
    void on_btn_emp_add_clicked();
    void on_btn_emp_edit_clicked();
    void on_btn_emp_del_clicked();
    void on_btn_emp_clr_clicked();

    // Employee table selection
    void on_tv_emp_clicked(const QModelIndex &index);

    // Employee search
    void on_search_emp_textChanged(const QString &text);

    // Employee list-tab actions
    void on_btn_emp_list_modify_clicked();
    void on_btn_emp_list_delete_clicked();
    void on_btn_emp_photo_clicked();

    // Employee role filter
    void on_combo_filter_role_currentTextChanged(const QString &role);
    void on_btn_emp_page_prev_clicked();
    void on_btn_emp_page_next_clicked();
    void on_btn_emp_action_new_clicked();
    void on_btn_emp_action_edit_clicked();
    void on_btn_emp_action_delete_clicked();
    void on_btn_emp_action_export_clicked();
    void on_btn_emp_action_refresh_clicked();

    // Employee inline editing
    void on_tv_emp_cellChanged(int row, int column);

    // Employee-Fabrication affectation
    void on_btn_emp_affect_clicked();
    void on_btn_emp_unaffect_clicked();
    void on_search_affect_emp_textChanged(const QString &text);
    void on_search_affect_fab_textChanged(const QString &text);

    // Employee statistics
    void on_btn_emp_stats_refresh_clicked();

    // Employee KNN classification
    void on_btn_emp_knn_run_clicked();

    // Employee Training Recommendations
    void on_combo_training_employee_currentIndexChanged(int index);
    void on_btn_training_analyze_clicked();
    void on_btn_training_refresh_clicked();

    // Bois CRUD slots
    void on_btn_bois_add_clicked();
    void on_btn_bois_edit_clicked();
    void on_btn_bois_del_clicked();
    void on_btn_bois_clr_clicked();

    // Bois table selection
    void on_tv_bois_clicked(const QModelIndex &index);

    // Bois search
    void on_search_bois_textChanged(const QString &text);

    // Bois list-tab actions
    void on_btn_bois_list_add_clicked();
    void on_btn_bois_list_delete_clicked();

    // Bois fournisseur filter
    void on_combo_filter_fournisseur_currentTextChanged(const QString &fournisseur);

    // Bois advanced features
    void on_tv_bois_cellChanged(int row, int column);
    void on_btn_bois_filter_clicked();
    void on_btn_bois_export_pdf_clicked();
    void on_btn_bois_stats_refresh_clicked();
    void on_btn_bois_recommend_clicked();
    void on_btn_bois_analyze_image_clicked();
    void on_btn_bois_prev_clicked();
    void on_btn_bois_next_clicked();

    // Fabrication CRUD slots
    void on_btn_fab_add_clicked();
    void on_btn_fab_edit_clicked();
    void on_btn_fab_del_clicked();
    void on_btn_fab_clr_clicked();

    // Fabrication table selection
    void on_tv_fab_clicked(const QModelIndex &index);

    // Fabrication search
    void on_search_fab_textChanged(const QString &text);

    // Fabrication list-tab actions
    void on_btn_fab_list_modify_clicked();
    void on_btn_fab_list_delete_clicked();
    void on_btn_fab_export_pdf_clicked();

    // Fabrication inline editing
    void on_tv_fab_cellChanged(int row, int column);

    // Fabrication etat filter
    void on_combo_filter_etat_currentTextChanged(const QString &etat);

    // Phase CRUD slots
    void on_btn_phase_add_clicked();
    void on_btn_phase_edit_clicked();
    void on_btn_phase_del_clicked();
    void on_btn_phase_clr_clicked();

    // Phase table selection
    void on_tv_phase_clicked(const QModelIndex &index);

    // Phase list-tab actions
    void on_btn_phase_list_modify_clicked();
    void on_btn_phase_list_delete_clicked();

    // Piece CRUD slots
    void on_btn_pc_add_clicked();
    void on_btn_pc_edit_clicked();
    void on_btn_pc_del_clicked();
    void on_btn_pc_clr_clicked();

    // Piece table selection
    void on_tv_pc_clicked(const QModelIndex &index);

    // Piece search
    void on_search_pc_textChanged(const QString &text);

    // Piece list-tab actions
    void on_btn_pc_list_modify_clicked();
    void on_btn_pc_list_delete_clicked();
    void on_btn_pc_export_pdf_clicked();

    // Piece inline editing
    void on_tv_pc_cellChanged(int row, int column);

    // Piece matiere filter
    void on_combo_filter_matiere_currentTextChanged(const QString &matiere);

    // Piece list sort
    void on_pc_sort_by_currentTextChanged(const QString &value);
    void on_pc_sort_order_currentTextChanged(const QString &value);

    // Piece source selection sync
    void on_pc_matiere_currentIndexChanged(int index);
    void on_pc_modele_currentIndexChanged(int index);

    // Piece statistics
    void on_btn_pc_stats_refresh_clicked();
    void on_pc_stats_sort_by_currentTextChanged(const QString &value);
    void on_pc_stats_sort_order_currentTextChanged(const QString &value);

    // Piece model-to-piece generation
    void on_btn_pc_model_import_clicked();
    void on_btn_pc_model_preview_clicked();
    void on_btn_pc_manual_generate_clicked();
    void on_btn_pc_auto_generate_clicked();

    // Piece nesting / manual cutting
    void on_btn_pc_nesting_run_clicked();
    void on_btn_pc_nesting_export_pdf_clicked();
    void on_btn_pc_nesting_export_csv_clicked();
    void on_btn_pc_manual_add_step_clicked();
    void on_btn_pc_manual_validate_clicked();
    void on_btn_pc_manual_export_clicked();
    void on_pc_ai_modele_source_currentIndexChanged(int index);
    void on_pc_manual_modele_currentIndexChanged(int index);
    void on_pc_manual_bois_currentIndexChanged(int index);
    void on_pc_manual_passes_valueChanged(int value);

    // Sign out actions
    void on_pushButton_8_clicked();
    void on_pushButton_10_clicked();

    // Header global search
    void on_lineEdit_textChanged(const QString &text);

    // Arduino slots
    void onArduinoBoardScanned(double length, double width, double thickness);
    void onArduinoConnectionStatusChanged(bool isConnected, const QString& message);
    void onArduinoDataReceived(const QString &distance, const QString &irState);
    void processFireArduinoFrame(const QString &frame);
    void on_btn_arduino_connect_clicked();
    void on_btn_arduino_disconnect_clicked();
    void onArduinoConnectionSucceeded(const QString &portName);
    void onArduinoConnectionFailed(const QString &portName, const QString &errorMessage);
    void onArduinoDisconnected();

    // Fire detection slots (ArduinoRanim)
    void onFireRanimAlarmReceived();
    void onFireRanimNormalReceived();
    void onFireRanimStatusMessage(const QString &message);
    void onFireRanimSerialError(const QString &message);
    void onFireRanimDatabaseError(const QString &message);

    // Test routines for fire alert banner
    void testFireAlertBannerNormal();
    void testFireAlertBannerFalseAlert();

private:
    Ui::smartcarpentry *ui;
    Employee empTmp;
    int selectedEmpId;  // ID of the employee selected in the table
    QString employeeSkillStatusFilter;
    bool employeeArchiveView;
    bool employeePermanentDeleteMode;
    ArduinoManager *arduinoManager;
    ArduinoHandler *arduinoHandler;
    ArduinoHandler *temperatureArduinoHandler;
    ArduinoHandler *modelePresenceArduinoHandler;
    ArduinoRanim *fireRanimHandler;
    QWidget *arduinoScannerTab;
    QLabel *arduinoStatusLabel;
    QLabel *arduinoDistanceLabel;
    QLabel *arduinoIrLabel;
    QComboBox *arduinoPortCombo;
    QLabel *arduinoPortsListLabel;
    QLabel *arduinoDetectionStatusLabel;
    QLabel *arduinoLiveFrameLabel;
    QLabel *arduinoLiveLengthLabel;
    QLabel *arduinoLiveWidthLabel;
    QLabel *arduinoLiveThicknessLabel;
    QLabel *arduinoKpiLengthLabel;
    QLabel *arduinoKpiWidthLabel;
    QLabel *arduinoKpiThicknessLabel;
    QLabel *arduinoKpiSurfaceLabel;
    QLabel *arduinoKpiQualityLabel;
    QProgressBar *arduinoScanProgressBar;
    QTimer *arduinoScanPulseTimer;
    QTextEdit *arduinoScanLogText;
    QVector<QFrame *> arduinoKpiCards;
    QStringList arduinoFrameQueue;
    double arduinoDetectedLengthMm;
    double arduinoDetectedWidthMm;
    double arduinoDetectedThicknessMm;
    double arduinoDetectedSurface;
    double arduinoDetectedVolume;
    bool arduinoPanelMetricsValid;
    bool arduinoIrPreviouslyDetected;
    QString arduinoPanelQualityStatus;
    QString arduinoPanelValidationMessage;
    QString arduinoLastPanelSignature;
    QDateTime arduinoLastScanDateTime;
    QDateTime arduinoLastPromptDateTime;
    QDateTime arduinoScanCycleStartedAt;
    bool arduinoScanCompletionAnnounced;
    QSqlQueryModel *employeeRawModel;
    QSortFilterProxyModel *employeeFilterProxy;
    int employeeCurrentPage;
    int employeePageSize;
    int affectEmpCurrentPage;
    int affectEmpPageSize;
    int affectFabCurrentPage;
    int affectFabPageSize;
    QPushButton *affectEmpPrevButton;
    QPushButton *affectEmpNextButton;
    QLabel *affectEmpPageLabel;
    QPushButton *affectFabPrevButton;
    QPushButton *affectFabNextButton;
    QLabel *affectFabPageLabel;
    QComboBox *affectSkillFilterCombo;
    QComboBox *affectSkillLevelCombo;
    QTableWidget *affectSkillMatrixTable;
    QLabel *affectSkillHintLabel;
    QLabel *affectEmployeeSkillsLabel;
    QTableWidget *affectEmployeeSkillsTable;

    Bois boisTmp;
    int selectedBoisId; // ID of the bois selected in the table
    int boisCurrentPage;
    int boisPageSize;
    void applyBoisPagination();
    void updateBoisPaginationLabel(int totalRows, int startRow, int endRow);

    Piece pieceTmp;
    int selectedPieceId; // ID of the piece selected in the table

    Fabrication fabTmp;
    int selectedFabId;  // ID of the fabrication selected in the table
    QSortFilterProxyModel *fabricationFilterProxy;
    int fabricationCurrentPage;
    int fabricationPageSize;
    QPushButton *fabricationPrevButton;
    QPushButton *fabricationNextButton;
    QLabel *fabricationPageLabel;
    QWidget *fabricationPageNumbersContainer;
    bool fabricationListUiReady;
    bool fabricationStatsUiReady;
    bool fabricationFireUiReady;
    bool fabricationTemperatureUiReady;
    bool fabricationAdvancedUiReady;
    bool fabricationComparisonUiReady;
    QLabel *fabricationStatsTotalValue;
    QLabel *fabricationStatsTotalHint;
    QLabel *fabricationStatsOnTimeValue;
    QProgressBar *fabricationStatsOnTimeBar;
    QLabel *fabricationStatsRiskValue;
    QLabel *fabricationStatsRiskHint;
    QTextEdit *fabricationStatsInsightText;
    QChartView *fabricationStatsStateChart;
    QChartView *fabricationStatsWorkloadChart;
    QWidget *fabricationFireTab;
    QComboBox *fabricationFirePortCombo;
    QPushButton *fabricationFireRefreshPortsButton;
    QPushButton *fabricationFireConnectButton;
    QLabel *fabricationFirePortStatusLabel;
    QLabel *fabricationFirePortLabel;
    QLabel *fabricationFireConnectionLabel;
    QWidget *fabricationFireBanner;
    QLabel *fabricationFireBannerTitle;
    QLabel *fabricationFireBannerMessage;
    QWidget *fireAlertPopupWindow;
    QLabel *fireAlertPopupTitle;
    QLabel *fireAlertPopupMessage;
    QLabel *fabricationFireCurrentStateValue;
    QLabel *fabricationFireLastAlertValue;
    QLabel *fabricationFireSessionCountValue;
    QLabel *fabricationFireFalseAlertCountValue;
    QLabel *fabricationFireDbSummaryLabel;
    QTableWidget *fabricationFireLogTable;
    QComboBox *fabricationTemperaturePortCombo;
    QPushButton *fabricationTemperatureRefreshPortsButton;
    QPushButton *fabricationTemperatureConnectButton;
    QLabel *fabricationTemperaturePortStatusLabel;
    QLabel *fabricationTemperaturePortLabel;
    QLabel *fabricationTemperatureConnectionLabel;
    QLabel *fabricationTemperatureLiveValue;
    QLabel *fabricationTemperatureFabricationIdLabel;
    QLabel *fabricationTemperatureLastLineLabel;
    QWidget *fabricationTemperatureBanner;
    QLabel *fabricationTemperatureBannerTitle;
    QLabel *fabricationTemperatureBannerMessage;
    QChartView *fabricationTemperatureChartView;
    QChart *fabricationTemperatureChart;
    QLineSeries *fabricationTemperatureSeries;
    QDateTimeAxis *fabricationTemperatureAxisX;
    QValueAxis *fabricationTemperatureAxisY;
    QTableWidget *fabricationTemperatureLogTable;
    QTimer *fabricationTemperatureDbPollTimer;
    double fabricationTemperatureLastReading;
    QDateTime fabricationTemperatureLastJournalMesureAt;
    bool fireAlarmActive;
    bool fireAlertConfirmed;
    int fireSessionAlertCount;
    int fireFalseAlertCount;
    int fireLastDetectionSeconds;
    QDateTime fireAlarmStartedAt;
    QDateTime fireLastAlertAt;
    QDateTime fireLastNormalAt;
    QString fireCurrentState;
    QString fireConnectedPortName;
    QComboBox *fabricationAdvancedCombo;
    QTableView *fabricationAdvancedPhaseTable;
    QLabel *fabricationAdvancedProgressLabel;
    QPushButton *phaseAddButton;
    QPushButton *phaseModifyButton;
    QPushButton *phaseDeleteButton;
    QLabel *fabricationCurrentChairPreview;
    QLabel *fabricationReferenceChairPreview;
    QLabel *fabricationCurrentChairFileLabel;
    QLabel *fabricationReferenceChairFileLabel;
    QTableWidget *fabricationComparisonTable;
    QLabel *fabricationComparisonVerdictLabel;
    QPushButton *fabricationComparisonAnalyzeButton;
    QString fabricationCurrentChairImagePath;
    QString fabricationReferenceChairImagePath;
    QString fabricationAnnotatedChairImagePath;
    bool fabricationComparisonScanPending;
    int fabricationComparisonScanLatencyMs;

    // Planification (scheduling) UI components
    QTableWidget *planificationPhaseTable;
    QLabel *planificationFabricationLabel;
    QTimer *planificationTimer;
    QMap<int, QLabel*> planificationPhaseTimers;  // Maps phase index to timer label
    void setupPlanificationTab();
    void refreshPlanificationPanel();
    void updatePlanificationDuration(int row);
    void updatePlanificationTimers();

    Phase phaseTmp;
    int selectedPhaseId;  // ID of the phase selected in the table

    // Affectation inline editing
    void on_tv_affect_emp_cellChanged(int row, int column);
    void on_tv_affect_fab_cellChanged(int row, int column);

    Modele modeleTmp;
    int selectedModeleId; // ID of the modele selected in the table
    ModeleFilterProxyModel *modeleFilterProxy;
    int modeleCurrentPage;
    int modelePageSize;
    QPushButton *modelePrevButton;
    QPushButton *modeleNextButton;
    QLabel *modelePageLabel;
    QWidget *modelePageNumbersContainer;
    QComboBox *modeleStatusFilterCombo;
    QComboBox *modeleSortFieldCombo;
    QComboBox *modeleSortOrderCombo;
    bool modeleListUiReady;
    bool modeleStatsUiReady;
    bool modeleArduinoUiReady;
    QComboBox *modeleArduinoPortCombo;
    QPushButton *modeleArduinoRefreshPortsButton;
    QPushButton *modeleArduinoConnectButton;
    QLabel *modeleArduinoPortStatusLabel;
    QLabel *modeleArduinoConnectionLabel;
    QLabel *modeleArduinoPresenceValue;
    QLabel *modeleArduinoLastFrameValue;
    QLabel *modeleArduinoRiskHintLabel;
    QWidget *modeleArduinoPresenceBanner;
    QLabel *modeleArduinoPresenceBannerTitle;
    QLabel *modeleArduinoPresenceBannerMessage;
    QTableWidget *modeleArduinoLogTable;
    bool modeleArduinoPieceDetected;
    QLabel *modeleStatsTotalValue;
    QLabel *modeleStatsTotalHint;
    QLabel *modeleStatsMarginValue;
    QProgressBar *modeleStatsMarginProgress;
    QLabel *modeleStatsTopCategoryValue;
    QLabel *modeleStatsTopCategoryHint;
    /** Synthese textuelle sous le camembert (remplace les barres dupliquees). */
    QLabel *modeleStatsStatusSummary;
    QChartView *modeleStatsStatusChart;
    QChartView *modeleStatsTrendChart;
    QString modeleEstimateImagePath;
    QPushButton *modeleEstimateImportButton;
    QPushButton *modeleEstimateClearButton;
    QLabel *modeleEstimatePreviewLabel;
    QLabel *modeleEstimateTypeValue;
    QLabel *modeleEstimatePriceValue;
    QLabel *modeleEstimateTimeValue;
    QLabel *modeleEstimateDimensionsValue;
    QLabel *modeleEstimateErrorLabel;
    QWidget *modeleEstimateResultCard;
    BudgetGltfViewerWidget *modeleBudgetGltfViewer;
    QWidget *modeleBudgetWebContainer;
    QLabel *modeleBudgetViewerHint;
    QPushButton *modeleBudgetOpenExternalViewerButton;
    QString modeleBudgetLastViewerUrl;
    QChartView *modelePerfChartView;
    QChart *modelePerfChart;
    QLineSeries *modelePerfCostSeries;
    QBarSeries *modelePerfCostBarSeries;
    QBarSet *modelePerfCostBarSet;
    ModeleCostInput modelePerfLastInput;
    ModeleCostEstimateResult modelePerfLastResult;
    bool modelePerfHasEstimate;
    QString currentUserEmail;
    
    // RFID components
    RfidManager *rfidManager;
    QTextBrowser *rfidLogBrowser;
    QWidget *employeeRfidTab;
    QLabel *employeeRfidMessageLabel;
    QLabel *employeeRfidUserValue;
    QLabel *employeeRfidEmployeeIdValue;
    QLabel *employeeRfidRoleValue;
    QLabel *employeeRfidCardUidValue;
    QLabel *employeeRfidCardStatusValue;
    QLabel *employeeRfidInsideValue;
    QLabel *employeeRfidIssueDateValue;
    QLabel *employeeRfidLastAccessValue;
    QLabel *employeeRfidLastStatusValue;
    QLabel *employeeRfidLogCountValue;
    QTableView *employeeRfidLogsTable;
    QSqlQueryModel *employeeRfidLogsModel;
    
protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void setupArduinoScannerUI();
    void connectArduinoSerial();
    void readArduinoData();
    bool parseArduinoFrame(const QString &frame);
    void calculatePanelMetrics();
    bool validatePanelDimensions(QString *errorMessage = nullptr) const;
    bool saveDetectedPanelToDatabase();
    bool completeArduinoScanWithLatency();
    void clearArduinoScanCycle();
    void updateArduinoScannerStatus(const QString &statusText, const QString &tone);
    void appendArduinoScannerLog(const QString &message);
    void updateArduinoKpiTheme(const QString &tone);
    void refreshArduinoPortList();

    // Helper: refresh the employee table
    void refreshEmployeeTable();
    void applyEmployeePagination();
    void updateEmployeePaginationLabel(int totalRows, int startRow, int endRow);
    QSet<int> loadEmployeeIdsForSkillStatus(const QString &statusFilter) const;
    QString normalizeSkillStatusFilter(const QString &statusFilter) const;
    void setEmployeeSkillStatusFilter(const QString &statusFilter);
    
    // Universal Premium UI setup
    void applyGlobalPremiumStyle(QTableView *tv, QLineEdit *search = nullptr, QPushButton *modify = nullptr, QPushButton *deleteBtn = nullptr);

    // Phase form dialog
    void showPhaseFormDialog(int idPhase);

    // Helper: clear employee form fields
    void clearEmployeeForm();

    // Helper: refresh the affectation tab tables
    void refreshAffectationTables();
    void setupAffectationPaginationUi();
    void applyAffectationPagination();
    void setupAffectationSkillUi();
    void refreshAffectationSkillFilters();
    void refreshAffectationSkillMatrix();
    void refreshAffectationEmployeeSkills(int employeeId);
    QString inferRequiredSkillForFabrication(int fabricationId, const QString &modeleName) const;
    bool validateEmployeeSkillForAssignment(int employeeId,
                                            const QString &skillName,
                                            const QString &requiredLevel,
                                            QString &errorMessage,
                                            bool &expiredCertificate) const;

    // Helper: refresh employee statistics
    void refreshEmployeeStats();

    // Helper: create chart views for employee statistics
    void setupEmployeeCharts();
    void setupEmployeeAssistantPanel();
    void repositionEmployeeAssistantPanel();
    void toggleEmployeeAssistantPanel();
    void askEmployeeAssistantFromPanel();
    void appendEmployeeAssistantMessage(const QString &speaker, const QString &message, bool isError = false);
    QString buildEmployeeAiContext() const;

    // Helper: Training recommendations engine
    struct TrainingRecommendation {
        QString trainingName;
        QString reason;
        QString priority;  // "High", "Medium", "Low"
        int estimatedHours;
        QString status;    // "Not Started", "In Progress", "Completed"
    };
    
    QVector<TrainingRecommendation> generateTrainingRecommendations(int employeeId) const;
    void refreshTrainingRecommendationsUI(int employeeId);
    void setupTrainingRecommendationsUI();
    void populateTrainingEmployeeCombo();

    // Helper: render KNN 2D classification chart
    void renderEmployeeKnnChart(const QVector<EmployeeKnnRecommendation> &recommendations,
                                double taskX,
                                double taskY,
                                int k);

    // Helper: setup wood form validation
    void setupBoisValidation();

    // Helper: load/select image for the bois IA analysis page
    void selectBoisAnalysisImage();
    void updateBoisAnalysisPreview(const QString &filePath);

    // Helper: refresh the bois table
    void refreshBoisTable();

    // Helper: clear bois form fields
    void clearBoisForm();

    // Helper: refresh the fabrication table
    void refreshFabricationTable();
    void setupFabricationListUi();
    void applyFabricationPagination();
    void updateFabricationPaginationLabel(int totalRows, int startRow, int endRow);
    void rebuildFabricationInlineActions();
    void setupFabricationStatsUi();
    void refreshFabricationStats();
    void setupFabricationFireUi();
    void refreshFabricationFireUi();
    void setupFabricationAdvancedUi();
    void refreshFabricationAdvancedPanel();
    void refreshFabricationAdvancedPhases();
    void setupFabricationComparisonUi();
    void selectFabricationChairImage(bool referenceImage);
    void updateFabricationChairPreview(const QString &filePath, bool referenceImage);
    void on_btn_fab_compare_analyze_clicked();
    void runFabricationChairComparisonScan();
    void appendFireEventLog(const QDateTime &timestamp,
                            const QString &eventType,
                            const QString &stateLabel,
                            const QString &message);
    void persistFireEvent(const QString &eventType,
                          const QString &stateLabel,
                          const QString &message,
                          const QDateTime &timestamp);
    bool recordFireFalseAlertIfPending(const QDateTime &timestamp);
    void updateFireBanner(bool visible,
                          const QString &title,
                          const QString &message,
                          const QString &tone);
    void loadFireStateFromDatabase();

    void setupFabricationTemperatureUi();
    void refreshFabricationTemperatureUi();
    void appendTemperatureEventLog(const QDateTime &timestamp,
                                   const QString &category,
                                   const QString &detail);
    void appendTemperatureChartPoint(double tempCelsius);
    void processTemperatureArduinoFrame(const QString &frame);

    // Helper: clear fabrication form fields
    void clearFabricationForm();

    // Helper: load modele options for fabrication form
    void loadFabricationModeleOptions();

    // Helper: refresh the modele table
    void refreshModeleTable();
    void setupModeleListUi();
    void applyModelePagination();
    void updateModelePaginationLabel(int totalRows, int startRow, int endRow);
    void rebuildModeleInlineActions();
    void setupModeleStatsUi();
    void setupModeleArduinoUi();
    void refreshModeleArduinoUi();
    void processModeleArduinoPresenceFrame(const QString &frame);
    void appendModeleArduinoLog(const QString &category, const QString &message);

    // Helper: clear modele form fields
    void clearModeleForm();

    // Helper: setup modele form comboboxes
    void setupModeleCombos();

    // Helper: refresh modele stats
    void refreshModeleStats();
    void setupModeleEstimationUi();
    void setupModeleBudgetUi();
    void setupModelePerformanceUi();
    void updateModelePerfCostChart(const ModeleCostEstimateResult &res);
    void updateModeleEstimationPreview(const QString &imagePath);
    bool runModeleAiEstimation(const QString &imagePath,
                               QJsonObject &result,
                               QString &errorMessage) const;
    bool saveModeleEstimationToDatabase(const QJsonObject &result,
                                        const QString &imagePath,
                                        QString &errorMessage);

    // Helper: refresh the piece table
    void refreshPieceTable();

    // Helper: clear piece form fields
    void clearPieceForm();

    // Helper: load available bois/modeles for piece form
    void loadPieceBoisOptions();
    void loadPieceModeleOptions();
    void updatePieceModelImagePreview(const QString &imagePath);
    void updatePieceManualCanvasHint();
    void setupPieceWorkflowUi();
    void renderPieceManualCanvas();
    void renderPieceNestingCanvas();
    void resetPieceGenerationState(bool clearNotes = false);
    void refreshPieceGenerationContext();
    void updatePieceWorkflowState();
    void applyPieceCompositeFilter();
    bool confirmPieceContextChange(int newModeleId, int newBoisId);
    void syncPieceContextSelectors(int modeleId, int boisId, QObject *origin = nullptr);
    QVector<PieceDraft> resolvePieceDraftsForContext(int modeleId,
                                                     int boisId,
                                                     bool allowStoredFallback,
                                                     bool *loadedFromStorage = nullptr,
                                                     QString *errorMessage = nullptr) const;
    QVector<PieceDraft> resolvePieceWorkingSetForContext(int modeleId,
                                                         int boisId,
                                                         bool *usingDraftDrafts = nullptr,
                                                         QString *errorMessage = nullptr) const;
    PieceWorkflowState buildPieceWorkflowState(PieceWorkflowStage stage,
                                               bool dirty) const;
    bool savePieceWorkflowState(PieceWorkflowStage stage,
                                bool dirty,
                                QString &errorMessage);
    void restorePieceWorkflowState(const PieceWorkflowState &state);
    void clearPieceWorkflowStateUi(bool clearNotes = false);
    bool startCurrentPieceRevision(QString &errorMessage);
    bool validateCurrentPieceAtelier(QString &errorMessage);
    bool openPieceCatalogDialog(int pieceId = -1);
    void setPieceDraftBufferState(const QVector<PieceDraft> &drafts,
                                  PieceDraftUiState state,
                                  const QStringList &warnings = QStringList());
    void markPieceDraftDirty();
    bool saveCurrentPieceDraft(QString &errorMessage);
    bool publishCurrentPieceDraft(bool replacePublished, QString &errorMessage);
    bool validateCurrentPieceDraft(QString &errorMessage) const;
    void loadCurrentPieceDraft();
    void discardCurrentPieceDraft();
    void applyRecommendedPieceNestingSettings(const QVector<PieceDraft> &drafts);
    QString buildPieceManualCutPlan(const QVector<PieceDraft> &drafts) const;
    QVector<PieceDraft> generatePieceDraftsFromModel(int modeleId,
                                                     int boisId,
                                                     bool manualMode,
                                                     const QString &manualNotes,
                                                     const QString &precision,
                                                     double toleranceMm,
                                                     bool detectEdges,
                                                     bool mergeSymmetry,
                                                     QString &summary,
                                                     QString &details) const;
    bool persistPieceDrafts(const QVector<PieceDraft> &drafts,
                            int modeleId,
                            int boisId,
                            bool replaceExisting,
                            QString &errorMessage);
    void setPieceGenerationResult(const QVector<PieceDraft> &drafts,
                                  const QString &summary,
                                  const QString &details);
    void runPieceNestingOptimization();
    void syncPieceAiGeneratorCombos();
    bool capturePieceVoiceText(QString &transcribedText) const;
    bool recordPieceVoiceSample(QByteArray &pcmData,
                                int &sampleRate,
                                int &channelCount,
                                QString &errorMessage) const;
    QString resolveVoskPythonExecutable() const;
    QString resolveVoskModelPath(QString &errorMessage) const;
    QString resolveVoskScriptPath(QString &errorMessage) const;
    bool transcribePieceVoiceWithVosk(const QByteArray &pcmData,
                                      int sampleRate,
                                      int channelCount,
                                      QString &transcribedText,
                                      QString &errorMessage) const;
    bool processPieceVoiceInput(bool autoInsert);
    bool extractPieceFromNaturalLanguage(const QString &input,
                                         PieceDraft &draft,
                                         QString &errorMessage,
                                         QStringList *warnings = nullptr,
                                         int *suggestedBoisId = nullptr) const;
    bool insertGeneratedPieceFromText(const QString &input, QString &errorMessage);
    void updatePieceAiPreview(const QString &message, bool isError = false);
    void appendPieceAiHistoryRow(const Piece &piece, const QString &modeleLabel, const QString &boisLabel);

    // Helper: refresh statistics widgets on piece page
    void refreshPieceStats();

    // Helper: apply sort on piece list
    void applyPieceListSort();

    // Helper: create chart views for piece statistics
    void setupPieceCharts();

    QChartView *pieceMaterialChartView;
    QChartView *pieceTrendChartView;
    PieceFilterProxyModel *pieceFilterProxy;
    QVector<PieceDraft> pieceDraftBuffer;
    QVector<PieceNestingPlacement> pieceNestingPlacements;
    QString pieceModelImagePath;
    QString pieceNestingLogBuffer;
    QStringList pieceDraftWarnings;
    PieceWorkflowState pieceWorkflowStateCache;
    PieceDraftUiState pieceDraftUiState;
    int pieceContextModeleId;
    int pieceContextBoisId;
    bool pieceInlineEditGuard;
    QLabel *pieceDraftStatusBanner;
    QLabel *pieceDraftWorkflowHint;
    QWidget *pieceDraftActionPanel;
    QWidget *pieceAiGeneratorTab;
    QTextEdit *pieceAiPromptEdit;
    QLabel *pieceAiPreviewLabel;
    QLabel *pieceAiStatusLabel;
    QComboBox *pieceAiModeleCombo;
    QComboBox *pieceAiBoisCombo;
    QPushButton *pieceAiExtractButton;
    QPushButton *pieceAiInsertButton;
    QPushButton *pieceAiVoiceRecordButton;
    QPushButton *pieceAiVoiceInsertButton;
    QTableWidget *pieceWidget;
    QTabWidget *pieceStatsPages;
    QWidget *pieceStatsOverviewPage;
    QWidget *pieceStatsMaterialsPage;
    QTabWidget *pieceNestingPages;
    QWidget *pieceNestingSetupPage;
    QWidget *pieceNestingOutputPage;
    QPushButton *pieceDraftLoadButton;
    QPushButton *pieceDraftSaveButton;
    QPushButton *pieceDraftGenerateButton;
    QPushButton *pieceDraftValidateButton;
    QPushButton *pieceDraftPublishButton;
    QPushButton *pieceDraftDiscardButton;
    QPushButton *pieceDraftRevisionButton;
    QPushButton *pieceListEditButton;
    QComboBox *pieceStatusFilterCombo;

    QChartView *empRoleChartView;
    QChartView *empFabChartView;
    QChartView *empSkillStatusChartView;
    QChartView *empKnnChartView;
    QSpinBox *settingsKnnKSpin;
    AIAssistantService *employeeAiAssistant;
    QPushButton *employeeAiFloatingButton;
    QFrame *employeeAiPanel;
    QTextEdit *employeeAiHistory;
    QTextEdit *employeeAiInput;
    QPushButton *employeeAiAskButton;
    QPushButton *employeeAiCloseButton;
    QLabel *employeeAiStatusLabel;
    bool employeeAiBusy;

    // Helper: create chart views for bois statistics
    void setupBoisCharts();

    QChartView *boisQualityPieChart;
    QChartView *boisStockBarChart;
    QChartView *boisSupplierPieChart;

    // Bois IA tab dynamically-created widgets
    QTableWidget *tw_bois_analysis;
    QTableWidget *tw_bois_recommendations;
    QLabel       *lb_bois_image_preview;
    QLabel       *lb_bois_image_filename;
    QLabel       *lb_bois_classe_verdict;
    QString       selectedBoisImagePath;

    // AI Assistant enhancements
    QString currentUserRole;
    QString currentUserName;
    int currentUserId;
    QMap<QString, QString> aiQueryCache;
    
    // Helper methods for enhanced AI assistant
    void loadCurrentUserProfile();
    QString parseAndExecuteDirectQuery(const QString &userQuery);
    QString buildSmartSuggestions() const;
    bool shouldIncludeSensitiveData() const;
    QString filterSensitiveDataFromContext(const QString &context) const;

    // AI Smart Nesting Optimizer
    void loadPublishedPiecesToNesting();
    NestingWidget *nestingWidget;
    QVector<PieceRect*> nestingPieces;
    QVector<Panel*> nestingPanels;

    // Helper: RFID tab
    void setupEmployeeRfidTab();
    void refreshEmployeeRfidTab();

    // Helper: execute sign out flow
    void performSignOut();
};

#endif // SMARTCARPENTRY_H
