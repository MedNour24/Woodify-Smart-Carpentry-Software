/********************************************************************************
** Form generated from reading UI file 'smartcarpentry.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMARTCARPENTRY_H
#define UI_SMARTCARPENTRY_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_smartcarpentry
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_4;
    QWidget *icon_only_widget;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_7;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_9;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QSpacerItem *verticalSpacer_3;
    QWidget *icon_text_widget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_7;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_8;
    QVBoxLayout *verticalLayout;
    QPushButton *pbbois;
    QPushButton *pbemployee;
    QPushButton *pbfabrication;
    QPushButton *pbmodele;
    QPushButton *pbpieces;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pbsettings;
    QPushButton *pushButton_10;
    QSpacerItem *verticalSpacer_5;
    QWidget *header_widget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QPushButton *pushButton_17;
    QLabel *label_4;
    QGridLayout *gridLayout_2;
    QPushButton *Profil_photo;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_2;
    QStackedWidget *stackedWidget;
    QWidget *Pemployee;
    QWidget *c_Pemp;
    QLabel *t_emp;
    QTabWidget *tabWidget_emp;
    QWidget *tab_emp_list;
    QVBoxLayout *vl_emp_list;
    QHBoxLayout *hl_emp_top;
    QLineEdit *search_emp;
    QLabel *lb_filter_role;
    QComboBox *combo_filter_role;
    QHBoxLayout *hl_emp_action_top;
    QPushButton *btn_emp_action_export;
    QPushButton *btn_emp_action_refresh;
    QSpacerItem *hs_emp_action_top_right;
    QTableView *tv_emp;
    QHBoxLayout *hl_emp_pagination;
    QSpacerItem *hs_emp_pag_left;
    QPushButton *btn_emp_page_prev;
    QWidget *emp_page_nums_container;
    QPushButton *btn_emp_page_next;
    QLabel *lb_emp_pagination;
    QSpacerItem *hs_emp_pag_right;
    QHBoxLayout *hl_emp_actions;
    QSpacerItem *hs_emp_left;
    QPushButton *btn_emp_list_modify;
    QPushButton *btn_emp_list_delete;
    QWidget *tab_emp_form;
    QVBoxLayout *vl_emp_form;
    QGroupBox *gf_emp;
    QGridLayout *fl_emp;
    QLabel *le1;
    QLineEdit *emp_cin;
    QLabel *le2;
    QLineEdit *emp_mail;
    QLabel *le3;
    QLineEdit *emp_password;
    QLabel *le4;
    QLineEdit *emp_nom;
    QLabel *le5;
    QLineEdit *emp_prenom;
    QLabel *le6;
    QDateEdit *emp_date_naissance;
    QLabel *le7;
    QComboBox *emp_role;
    QLabel *le10;
    QHBoxLayout *hl_emp_photo;
    QLineEdit *emp_face_id;
    QPushButton *btn_emp_photo;
    QHBoxLayout *bl_emp;
    QPushButton *btn_emp_add;
    QPushButton *btn_emp_edit;
    QPushButton *btn_emp_del;
    QPushButton *btn_emp_clr;
    QSpacerItem *verticalSpacer_emp;
    QWidget *tab_emp_affectation;
    QVBoxLayout *vl_emp_affect;
    QHBoxLayout *hl_affect_panels;
    QGroupBox *gb_affect_emp;
    QVBoxLayout *vl_affect_emp_inner;
    QLineEdit *search_affect_emp;
    QTableView *tv_affect_emp;
    QGroupBox *gb_affect_fab;
    QVBoxLayout *vl_affect_fab_inner;
    QLineEdit *search_affect_fab;
    QTableView *tv_affect_fab;
    QHBoxLayout *hl_affect_actions;
    QSpacerItem *hs_affect_left;
    QPushButton *btn_emp_affect;
    QPushButton *btn_emp_unaffect;
    QSpacerItem *hs_affect_right;
    QWidget *tab_emp_stats;
    QVBoxLayout *vl_emp_stats;
    QHBoxLayout *hl_emp_stats_cards;
    QGroupBox *gb_emp_stats_total;
    QVBoxLayout *vl_emp_stats_total;
    QLabel *lb_emp_stats_total_title;
    QLabel *lb_emp_stats_total_value;
    QLabel *lb_emp_stats_total_hint;
    QGroupBox *gb_emp_stats_affect;
    QVBoxLayout *vl_emp_stats_affect;
    QLabel *lb_emp_stats_affect_title;
    QLabel *lb_emp_stats_affected_value;
    QProgressBar *pb_emp_stats_affected;
    QGroupBox *gb_emp_stats_toprole;
    QVBoxLayout *vl_emp_stats_toprole;
    QLabel *lb_emp_stats_toprole_title;
    QLabel *lb_emp_stats_toprole_value;
    QLabel *lb_emp_stats_toprole_hint;
    QHBoxLayout *hl_emp_stats_body;
    QGroupBox *gb_emp_stats_roles;
    QVBoxLayout *vl_emp_stats_roles;
    QLabel *lb_emp_stats_role_1;
    QProgressBar *pb_emp_stats_role_1;
    QLabel *lb_emp_stats_role_2;
    QProgressBar *pb_emp_stats_role_2;
    QLabel *lb_emp_stats_role_3;
    QProgressBar *pb_emp_stats_role_3;
    QWidget *emp_stats_role_chart_container;
    QSpacerItem *vs_emp_stats_roles;
    QGroupBox *gb_emp_stats_pipeline;
    QVBoxLayout *vl_emp_stats_pipeline;
    QLabel *lb_emp_stats_pipeline_title;
    QWidget *emp_stats_fab_chart_container;
    QTextEdit *txt_emp_stats_pipeline;
    QPushButton *btn_emp_stats_refresh;
    QSpacerItem *vs_emp_stats_pipeline;
    QWidget *tab_emp_knn;
    QVBoxLayout *vl_emp_knn;
    QGroupBox *gb_emp_knn_inputs;
    QGridLayout *gl_emp_knn_inputs;
    QLabel *lb_emp_knn_task_x;
    QDoubleSpinBox *emp_knn_task_x;
    QLabel *lb_emp_knn_task_y;
    QDoubleSpinBox *emp_knn_task_y;
    QLabel *lb_emp_knn_k;
    QSpinBox *emp_knn_k;
    QLabel *lb_emp_knn_role;
    QComboBox *emp_knn_role;
    QLabel *lb_emp_knn_source_title;
    QLabel *lb_emp_knn_source_type;
    QComboBox *emp_knn_source_type;
    QLabel *lb_emp_knn_source_id;
    QSpinBox *emp_knn_source_id;
    QPushButton *btn_emp_knn_run;
    QLabel *lb_emp_knn_summary;
    QFrame *emp_knn_chart_container;
    QTableView *tv_emp_knn;
    QWidget *tab_emp_training;
    QVBoxLayout *vl_emp_training;
    QGroupBox *gb_training_employee_select;
    QHBoxLayout *hl_training_select;
    QComboBox *combo_training_employee;
    QPushButton *btn_training_analyze;
    QPushButton *btn_training_refresh;
    QSpacerItem *horizontalSpacer_training;
    QGroupBox *gb_training_profile;
    QGridLayout *gl_training_profile;
    QLabel *lb_training_role;
    QLabel *lb_training_role_value;
    QLabel *lb_training_level;
    QLabel *lb_training_level_value;
    QLabel *lb_training_seniority;
    QLabel *lb_training_seniority_value;
    QLabel *lb_training_skills_count;
    QLabel *lb_training_skills_count_value;
    QGroupBox *gb_training_recommendations;
    QVBoxLayout *vl_training_recommendations;
    QTableWidget *tw_training_recommendations;
    QGroupBox *gb_training_current;
    QVBoxLayout *vl_training_completed;
    QTableWidget *tw_training_completed;
    QSpacerItem *verticalSpacer_training;
    QWidget *Pbois;
    QWidget *c_Pbois;
    QLabel *t_bois;
    QTabWidget *tabWidget_bois;
    QWidget *tab_bois_list;
    QVBoxLayout *vl_bois_list;
    QHBoxLayout *hl_bois_top;
    QLineEdit *search_bois;
    QLabel *lb_filter_fournisseur;
    QComboBox *combo_filter_fournisseur;
    QHBoxLayout *hl_bois_sort;
    QLabel *lb_bois_sort;
    QComboBox *cb_bois_sort_field;
    QComboBox *cb_bois_sort_order;
    QSpacerItem *hs_bois_sort;
    QTableView *tv_bois;
    QHBoxLayout *hl_bois_pagination;
    QSpacerItem *hs_bois_pag_left;
    QPushButton *btn_bois_prev;
    QWidget *bois_page_nums_container;
    QPushButton *btn_bois_next;
    QLabel *lb_bois_pagination;
    QSpacerItem *hs_bois_pag_right;
    QHBoxLayout *hl_bois_actions;
    QSpacerItem *hs_bois_left;
    QPushButton *btn_bois_list_add;
    QPushButton *btn_bois_list_delete;
    QPushButton *btn_bois_export_pdf;
    QWidget *tab_bois_stats;
    QVBoxLayout *vl_bois_stats;
    QHBoxLayout *hl_bois_stats_top;
    QLabel *lb_bois_stats_title;
    QSpacerItem *hs_bois_stats_spacer;
    QPushButton *btn_bois_stats_refresh;
    QFrame *tab_bois_stats_charts;
    QWidget *tab_bois_ai;
    QVBoxLayout *vl_bois_ai;
    QGroupBox *gb_bois_recommend;
    QVBoxLayout *vl_bois_recommend;
    QLabel *lb_bois_recommend_info;
    QPushButton *btn_bois_recommend;
    QLabel *lb_bois_recommend_result;
    QGroupBox *gb_bois_image_analyse;
    QVBoxLayout *vl_bois_image;
    QLabel *lb_bois_image_info;
    QPushButton *btn_bois_analyze_image;
    QSpacerItem *vs_bois_ai_spacer;
    QWidget *tab_bois_form;
    QVBoxLayout *vl_bois_form;
    QGroupBox *gf_bois;
    QGridLayout *fl_bois;
    QLabel *lb1;
    QLineEdit *bois_nom;
    QLabel *lb2;
    QDoubleSpinBox *bois_longueur;
    QLabel *lb3;
    QDoubleSpinBox *bois_largeur;
    QLabel *lb4;
    QSpinBox *bois_quantite;
    QLabel *lb5;
    QDoubleSpinBox *bois_prix;
    QLabel *lb6;
    QComboBox *bois_fournisseur;
    QHBoxLayout *bl_bois;
    QPushButton *btn_bois_add;
    QPushButton *btn_bois_edit;
    QPushButton *btn_bois_del;
    QPushButton *btn_bois_clr;
    QSpacerItem *verticalSpacer_bois;
    QWidget *Pfabrication;
    QWidget *c_Pfab;
    QLabel *t_fab;
    QTabWidget *tabWidget_fab;
    QWidget *tab_fab_list;
    QVBoxLayout *vl_fab_list;
    QHBoxLayout *hl_fab_top;
    QLineEdit *search_fab;
    QLabel *lb_filter_etat;
    QComboBox *combo_filter_etat;
    QTableView *tv_fab;
    QHBoxLayout *hl_fab_actions;
    QSpacerItem *hs_fab_left;
    QPushButton *btn_fab_list_modify;
    QPushButton *btn_fab_list_delete;
    QPushButton *btn_fab_export_pdf;
    QWidget *tab_fab_form;
    QVBoxLayout *vl_fab_form;
    QGroupBox *gf_fab;
    QGridLayout *fl_fab;
    QLabel *lf0;
    QLineEdit *fab_id;
    QLabel *lf1;
    QDateEdit *fab_date_debut;
    QLabel *lf2;
    QDateEdit *fab_date_fin_prevue;
    QLabel *lf3;
    QDateEdit *fab_date_fin_reelle;
    QLabel *lf4;
    QSpinBox *fab_nb_personnes;
    QLabel *lf5;
    QComboBox *fab_etat;
    QLabel *lf6;
    QComboBox *fab_idModele;
    QHBoxLayout *bl_fab;
    QPushButton *btn_fab_add;
    QPushButton *btn_fab_edit;
    QPushButton *btn_fab_del;
    QPushButton *btn_fab_clr;
    QSpacerItem *vs_fab_btm;
    QWidget *Pmodele;
    QWidget *c_Pmod;
    QLabel *t_mod;
    QTabWidget *tabWidget_mod;
    QWidget *tab_mod_list;
    QVBoxLayout *vl_mod_list;
    QGroupBox *gl_mod;
    QVBoxLayout *ll_mod;
    QLineEdit *search_mod;
    QTableView *tv_mod;
    QPushButton *btn_mod_export_pdf;
    QWidget *tab_mod_form;
    QVBoxLayout *vl_mod_form;
    QGroupBox *gf_mod;
    QGridLayout *fl_mod;
    QLabel *lm0;
    QLineEdit *mod_id;
    QLabel *lm1;
    QLineEdit *mod_nom;
    QLabel *lm2;
    QComboBox *mod_categorie;
    QLabel *lm3;
    QLineEdit *mod_dim;
    QLabel *lm4;
    QComboBox *mod_type;
    QLabel *lm5;
    QDoubleSpinBox *mod_prix;
    QLabel *lm6;
    QTextEdit *mod_desc;
    QLabel *lm7;
    QDoubleSpinBox *mod_cout;
    QWidget *bb_mod;
    QHBoxLayout *bl_mod;
    QPushButton *btn_mod_add;
    QPushButton *btn_mod_edit;
    QPushButton *btn_mod_del;
    QPushButton *btn_mod_clr;
    QSpacerItem *vs_mod_bottom;
    QWidget *tab_mod_stats;
    QVBoxLayout *vl_mod_stats;
    QHBoxLayout *hl_mod_stats_top;
    QGroupBox *gb_mod_stats_total;
    QGridLayout *gl_mod_stats_total;
    QLabel *lb_mod_stats_total_title;
    QLabel *lb_mod_stats_total;
    QLabel *lb_mod_stats_avg_price_title;
    QLabel *lb_mod_stats_avg_price;
    QGroupBox *gb_mod_stats_price;
    QGridLayout *gl_mod_stats_price;
    QLabel *lb_mod_stats_min_price_title;
    QLabel *lb_mod_stats_min_price;
    QLabel *lb_mod_stats_max_price_title;
    QLabel *lb_mod_stats_max_price;
    QGroupBox *gb_mod_stats_cost;
    QGridLayout *gl_mod_stats_cost;
    QLabel *lb_mod_stats_avg_cost_title;
    QLabel *lb_mod_stats_avg_cost;
    QHBoxLayout *hl_mod_stats_tables;
    QGroupBox *gb_mod_stats_top;
    QVBoxLayout *vl_mod_stats_top;
    QTableView *tv_mod_stats_top;
    QGroupBox *gb_mod_stats_cat;
    QVBoxLayout *vl_mod_stats_cat;
    QTableView *tv_mod_stats_cat;
    QHBoxLayout *hl_mod_stats_charts;
    QGroupBox *gb_mod_stats_chart_cat;
    QVBoxLayout *vl_mod_stats_chart_cat;
    QChartView *mod_stats_chart_categories;
    QGroupBox *gb_mod_stats_chart_price;
    QVBoxLayout *vl_mod_stats_chart_price;
    QChartView *mod_stats_chart_price;
    QPushButton *btn_mod_stats_refresh;
    QSpacerItem *vs_mod_stats_bottom;
    QWidget *tab_mod_estimation;
    QVBoxLayout *vl_mod_estimation;
    QGroupBox *gb_mod_estimation;
    QVBoxLayout *vl_mod_estimation_box;
    QLabel *lb_mod_estimate_hint;
    QPushButton *btn_mod_estimate_image;
    QSpacerItem *vs_mod_estimation_bottom;
    QWidget *tab_mod_budget;
    QVBoxLayout *vl_mod_budget;
    QGroupBox *gb_mod_budget;
    QGridLayout *gl_mod_budget;
    QLabel *lb_mod_budget_label;
    QDoubleSpinBox *ds_mod_budget;
    QPushButton *btn_mod_budget_generate;
    QLabel *lb_mod_budget_result;
    QSpacerItem *vs_mod_budget_bottom;
    QWidget *tab_mod_performance;
    QVBoxLayout *vl_mod_perf_root;
    QHBoxLayout *hl_mod_perf;
    QGroupBox *gb_mod_perf_entry;
    QGridLayout *gl_mod_perf_entry;
    QLabel *lb_mod_perf_type;
    QComboBox *cb_mod_perf_type;
    QLabel *lb_mod_perf_dimensions;
    QLineEdit *le_mod_perf_dimensions;
    QLabel *lb_mod_perf_materiau;
    QComboBox *cb_mod_perf_materiau;
    QLabel *lb_mod_perf_complexite;
    QComboBox *cb_mod_perf_complexite;
    QLabel *lb_mod_perf_quantite;
    QSpinBox *sb_mod_perf_quantite;
    QPushButton *btn_mod_perf_calculate;
    QGroupBox *gb_mod_perf_results;
    QVBoxLayout *vl_mod_perf_results;
    QTextBrowser *tb_mod_perf_result;
    QGroupBox *gb_mod_perf_chart;
    QVBoxLayout *vl_mod_perf_chart;
    QWidget *w_mod_perf_chart_placeholder;
    QHBoxLayout *hl_mod_perf_footer;
    QPushButton *btn_mod_perf_export_pdf;
    QSpacerItem *hs_mod_perf_footer;
    QWidget *Ppieces;
    QWidget *c_Ppc;
    QLabel *t_pc;
    QTabWidget *tabWidget_pc;
    QWidget *tab_pc_list;
    QVBoxLayout *vl_pc_list;
    QHBoxLayout *hl_pc_top;
    QLineEdit *search_pc;
    QLabel *lb_filter_matiere;
    QComboBox *combo_filter_matiere;
    QLabel *lb_pc_sort_by;
    QComboBox *pc_sort_by;
    QComboBox *pc_sort_order;
    QTableView *tv_pc;
    QHBoxLayout *hl_pc_actions;
    QSpacerItem *hs_pc_left;
    QPushButton *btn_pc_list_modify;
    QPushButton *btn_pc_list_delete;
    QPushButton *btn_pc_export_pdf;
    QWidget *tab_pc_form;
    QVBoxLayout *vl_pc_form;
    QGroupBox *gf_pc;
    QGridLayout *fl_pc;
    QLabel *lp0;
    QLineEdit *pc_id;
    QLabel *lp1;
    QLineEdit *pc_nom;
    QLabel *lp2;
    QLineEdit *pc_code;
    QLabel *lp3;
    QSpinBox *pc_qty;
    QLabel *lp4;
    QComboBox *pc_matiere;
    QLabel *lp5;
    QDoubleSpinBox *pc_long;
    QLabel *lp6;
    QDoubleSpinBox *pc_larg;
    QLabel *lp7;
    QDoubleSpinBox *pc_epais;
    QLabel *lp8;
    QComboBox *pc_modele;
    QLabel *lp9;
    QSpinBox *pc_idModele;
    QLabel *lp10;
    QSpinBox *pc_idBois;
    QHBoxLayout *bl_pc;
    QPushButton *btn_pc_add;
    QPushButton *btn_pc_edit;
    QPushButton *btn_pc_del;
    QPushButton *btn_pc_clr;
    QSpacerItem *verticalSpacer_pc;
    QWidget *tab_pc_stats;
    QVBoxLayout *vl_pc_stats;
    QHBoxLayout *hl_pc_stats_cards;
    QGroupBox *gb_pc_stats_total;
    QVBoxLayout *vl_pc_stats_total;
    QLabel *lb_pc_stats_total_title;
    QLabel *lb_pc_stats_total_value;
    QLabel *lb_pc_stats_total_hint;
    QGroupBox *gb_pc_stats_yield;
    QVBoxLayout *vl_pc_stats_yield;
    QLabel *lb_pc_stats_yield_title;
    QLabel *lb_pc_stats_yield_value;
    QProgressBar *pb_pc_stats_yield;
    QGroupBox *gb_pc_stats_loss;
    QVBoxLayout *vl_pc_stats_loss;
    QLabel *lb_pc_stats_loss_title;
    QLabel *lb_pc_stats_loss_value;
    QLabel *lb_pc_stats_loss_hint;
    QHBoxLayout *hl_pc_stats_body;
    QGroupBox *gb_pc_stats_materials;
    QVBoxLayout *vl_pc_stats_materials;
    QHBoxLayout *hl_pc_stats_sort;
    QLabel *lb_pc_stats_sort;
    QComboBox *pc_stats_sort_by;
    QComboBox *pc_stats_sort_order;
    QLabel *lb_pc_stats_mat_1;
    QProgressBar *pb_pc_stats_mat_1;
    QLabel *lb_pc_stats_mat_2;
    QProgressBar *pb_pc_stats_mat_2;
    QLabel *lb_pc_stats_mat_3;
    QProgressBar *pb_pc_stats_mat_3;
    QWidget *pc_stats_material_chart_container;
    QGroupBox *gb_pc_stats_pipeline;
    QVBoxLayout *vl_pc_stats_pipeline;
    QLabel *lb_pc_stats_pipeline_title;
    QWidget *pc_stats_trend_chart_container;
    QTextEdit *txt_pc_stats_pipeline;
    QPushButton *btn_pc_stats_refresh;
    QWidget *tab_pc_model_ai;
    QVBoxLayout *vl_pc_model_ai;
    QTabWidget *tabWidget_pc_model_pages;
    QWidget *tab_pc_model_prepare;
    QVBoxLayout *vl_pc_model_prepare_page;
    QHBoxLayout *hl_pc_model_top;
    QGroupBox *gb_pc_model_source;
    QVBoxLayout *vl_pc_model_source;
    QLabel *lb_pc_model_dropzone;
    QHBoxLayout *hl_pc_model_import;
    QPushButton *btn_pc_model_import;
    QPushButton *btn_pc_model_preview;
    QGroupBox *gb_pc_model_settings;
    QGridLayout *gl_pc_model_settings;
    QLabel *lb_pc_model_selected;
    QComboBox *pc_ai_modele_source;
    QLabel *lb_pc_model_precision;
    QComboBox *pc_ai_precision;
    QLabel *lb_pc_model_tolerance;
    QDoubleSpinBox *pc_ai_tolerance;
    QCheckBox *chk_pc_ai_edges;
    QCheckBox *chk_pc_ai_merge;
    QTabWidget *tabWidget_pc_generation_modes;
    QWidget *tab_pc_generation_manual;
    QVBoxLayout *vl_pc_generation_manual;
    QTextEdit *txt_pc_manual_notes;
    QPushButton *btn_pc_manual_generate;
    QWidget *tab_pc_generation_auto;
    QVBoxLayout *vl_pc_generation_auto;
    QLabel *lb_pc_auto_hint;
    QPushButton *btn_pc_auto_generate;
    QWidget *tab_pc_model_draft;
    QVBoxLayout *vl_pc_model_draft_page;
    QGroupBox *gb_pc_model_result;
    QVBoxLayout *vl_pc_model_result;
    QLabel *lb_pc_model_result_summary;
    QTextEdit *txt_pc_model_result;
    QWidget *tab_pc_nesting;
    QVBoxLayout *vl_pc_nesting;
    QHBoxLayout *hl_pc_nesting_top;
    QGroupBox *gb_pc_nesting_sheet;
    QGridLayout *gl_pc_nesting_sheet;
    QCheckBox *chk_pc_nesting_save_scrap;
    QDoubleSpinBox *pc_sheet_gap;
    QLabel *lb_pc_nesting_width;
    QDoubleSpinBox *pc_sheet_largeur;
    QDoubleSpinBox *pc_sheet_longueur;
    QLabel *lb_pc_nesting_length;
    QComboBox *pc_nesting_algo;
    QCheckBox *chk_pc_nesting_rotate;
    QLabel *lb_pc_nesting_algo;
    QLabel *lb_pc_nesting_gap;
    QGroupBox *gb_pc_nesting_metrics;
    QVBoxLayout *vl_pc_nesting_metrics;
    QLabel *lb_pc_nesting_eff;
    QProgressBar *pb_pc_nesting_eff;
    QLabel *lb_pc_nesting_scrap;
    QProgressBar *pb_pc_nesting_scrap;
    QLabel *lb_pc_nesting_summary;
    QPushButton *btn_pc_nesting_run;
    QHBoxLayout *hl_pc_nesting_bottom;
    QGroupBox *gb_pc_nesting_canvas;
    QVBoxLayout *vl_pc_nesting_canvas;
    QLabel *lb_pc_nesting_canvas;
    QGroupBox *gb_pc_nesting_actions;
    QVBoxLayout *vl_pc_nesting_actions;
    QPushButton *btn_pc_nesting_export_pdf;
    QSpacerItem *vs_pc_nesting_actions;
    QPushButton *btn_pc_nesting_export_csv;
    QTextEdit *txt_pc_nesting_log;
    QWidget *tab_pc_manual_cut;
    QVBoxLayout *vl_pc_manual_cut;
    QHBoxLayout *hl_pc_manual_top;
    QGroupBox *gb_pc_manual_source;
    QGridLayout *gl_pc_manual_source;
    QLabel *lb_pc_manual_modele;
    QComboBox *pc_manual_modele;
    QLabel *lb_pc_manual_bois;
    QComboBox *pc_manual_bois;
    QLabel *lb_pc_manual_passes;
    QSpinBox *pc_manual_passes;
    QCheckBox *chk_pc_manual_keep_offcuts;
    QCheckBox *chk_pc_manual_axis_lock;
    QGroupBox *gb_pc_manual_canvas;
    QVBoxLayout *vl_pc_manual_canvas;
    QLabel *lb_pc_manual_canvas;
    QGroupBox *gb_pc_manual_plan;
    QVBoxLayout *vl_pc_manual_plan;
    QTextEdit *txt_pc_manual_cut_plan;
    QHBoxLayout *hl_pc_manual_actions;
    QPushButton *btn_pc_manual_add_step;
    QPushButton *btn_pc_manual_validate;
    QPushButton *btn_pc_manual_export;
    QWidget *Psettings;
    QWidget *c_Pset;
    QLabel *t_set;
    QGroupBox *gf_set;
    QGridLayout *fl_set;
    QLabel *ls0;
    QLineEdit *set_host;
    QLabel *ls1;
    QLineEdit *set_port;
    QLabel *ls2;
    QLineEdit *set_db;
    QLabel *ls3;
    QLineEdit *set_user;
    QLabel *ls4;
    QLineEdit *set_pass;
    QWidget *bb_set;
    QHBoxLayout *bl_set;
    QPushButton *btn_test_conn;
    QPushButton *btn_save_settings;
    QGroupBox *gf_app;
    QVBoxLayout *al_set;
    QCheckBox *ttsenable;
    QCheckBox *qrenable;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *smartcarpentry)
    {
        if (smartcarpentry->objectName().isEmpty())
            smartcarpentry->setObjectName("smartcarpentry");
        smartcarpentry->resize(1550, 867);
        centralwidget = new QWidget(smartcarpentry);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(11, 10, 1531, 831));
        gridLayout_4 = new QGridLayout(layoutWidget);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        icon_only_widget = new QWidget(layoutWidget);
        icon_only_widget->setObjectName("icon_only_widget");
        icon_only_widget->setMaximumSize(QSize(100, 16777215));
        icon_only_widget->setStyleSheet(QString::fromUtf8("QWidget{\n"
"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0c0c1d, stop:1 #12122a);\n"
"border-right: 1px solid rgba(99,102,241,0.12);\n"
"}"));
        verticalLayout_5 = new QVBoxLayout(icon_only_widget);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_6 = new QLabel(icon_only_widget);
        label_6->setObjectName("label_6");
        label_6->setMaximumSize(QSize(81, 61));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/icons/logo.png")));
        label_6->setScaledContents(false);

        verticalLayout_5->addWidget(label_6);

        verticalSpacer_7 = new QSpacerItem(20, 69, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_7);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        pushButton_3 = new QPushButton(icon_only_widget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setMaximumSize(QSize(91, 37));
        pushButton_3->setMouseTracking(true);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 10px;\n"
"margin: 3px 6px;\n"
"padding: 8px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(99,102,241,0.12);\n"
"border-radius: 10px;\n"
"}\n"
"QPushButton:checked{\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(99,102,241,0.25), stop:1 rgba(139,92,246,0.15));\n"
"border-left: 3px solid qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #818cf8, stop:1 #6366f1);\n"
"border-radius: 10px;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/bois.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon.addFile(QString::fromUtf8(":/icons/bois_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_3->setIcon(icon);
        pushButton_3->setIconSize(QSize(100, 30));
        pushButton_3->setCheckable(true);
        pushButton_3->setAutoRepeat(false);
        pushButton_3->setAutoExclusive(false);

        verticalLayout_2->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(icon_only_widget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setMaximumSize(QSize(91, 37));
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/employee.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon1.addFile(QString::fromUtf8(":/icons/employee_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_4->setIcon(icon1);
        pushButton_4->setIconSize(QSize(100, 32));
        pushButton_4->setCheckable(true);
        pushButton_4->setAutoRepeat(false);
        pushButton_4->setAutoExclusive(false);

        verticalLayout_2->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(icon_only_widget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setMaximumSize(QSize(91, 37));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/fabrication.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon2.addFile(QString::fromUtf8(":/icons/fabrication_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_5->setIcon(icon2);
        pushButton_5->setIconSize(QSize(100, 35));
        pushButton_5->setCheckable(true);
        pushButton_5->setAutoExclusive(false);

        verticalLayout_2->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(icon_only_widget);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setMaximumSize(QSize(91, 37));
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/model.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon3.addFile(QString::fromUtf8(":/icons/model_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_6->setIcon(icon3);
        pushButton_6->setIconSize(QSize(100, 40));
        pushButton_6->setCheckable(true);
        pushButton_6->setAutoExclusive(false);

        verticalLayout_2->addWidget(pushButton_6);

        pushButton_9 = new QPushButton(icon_only_widget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setMaximumSize(QSize(91, 37));
        pushButton_9->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/piece.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon4.addFile(QString::fromUtf8(":/icons/piece_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_9->setIcon(icon4);
        pushButton_9->setIconSize(QSize(30, 40));
        pushButton_9->setCheckable(true);
        pushButton_9->setAutoExclusive(false);

        verticalLayout_2->addWidget(pushButton_9);


        verticalLayout_5->addLayout(verticalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 238, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        pushButton_7 = new QPushButton(icon_only_widget);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setMaximumSize(QSize(91, 27));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/settingssmall1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon5.addFile(QString::fromUtf8(":/icons/settingssmall1_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_7->setIcon(icon5);
        pushButton_7->setIconSize(QSize(100, 22));
        pushButton_7->setCheckable(true);
        pushButton_7->setChecked(false);
        pushButton_7->setAutoExclusive(false);

        verticalLayout_3->addWidget(pushButton_7);

        pushButton_8 = new QPushButton(icon_only_widget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setMaximumSize(QSize(91, 27));
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/signoutsmall1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon6.addFile(QString::fromUtf8(":/icons/signoutsmall1_blue.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_8->setIcon(icon6);
        pushButton_8->setIconSize(QSize(100, 20));
        pushButton_8->setCheckable(true);
        pushButton_8->setAutoExclusive(false);

        verticalLayout_3->addWidget(pushButton_8);


        verticalLayout_5->addLayout(verticalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 91, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        gridLayout_4->addWidget(icon_only_widget, 0, 0, 2, 1);

        icon_text_widget = new QWidget(layoutWidget);
        icon_text_widget->setObjectName("icon_text_widget");
        icon_text_widget->setStyleSheet(QString::fromUtf8("QWidget{\n"
"background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0c0c1d, stop:1 #12122a);\n"
"color: rgba(255,255,255,0.92);\n"
"border-right: 1px solid rgba(99,102,241,0.12);\n"
"}"));
        verticalLayout_6 = new QVBoxLayout(icon_text_widget);
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_7 = new QLabel(icon_text_widget);
        label_7->setObjectName("label_7");
        label_7->setMaximumSize(QSize(81, 61));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/icons/logo.png")));
        label_7->setScaledContents(false);

        horizontalLayout->addWidget(label_7);

        label_3 = new QLabel(icon_text_widget);
        label_3->setObjectName("label_3");
        label_3->setStyleSheet(QString::fromUtf8("    font-family: 'Segoe UI', 'Inter', sans-serif;\n"
"    font-weight: 600;\n"
"    letter-spacing: 0.5px;\n"
""));

        horizontalLayout->addWidget(label_3);


        verticalLayout_6->addLayout(horizontalLayout);

        verticalSpacer_8 = new QSpacerItem(20, 108, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_8);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pbbois = new QPushButton(icon_text_widget);
        pbbois->setObjectName("pbbois");
        pbbois->setMaximumSize(QSize(211, 37));
        pbbois->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/bois_2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon7.addFile(QString::fromUtf8(":/icons/bois_2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbbois->setIcon(icon7);
        pbbois->setIconSize(QSize(120, 120));
        pbbois->setCheckable(true);
        pbbois->setAutoRepeat(false);
        pbbois->setAutoExclusive(false);

        verticalLayout->addWidget(pbbois);

        pbemployee = new QPushButton(icon_text_widget);
        pbemployee->setObjectName("pbemployee");
        pbemployee->setMaximumSize(QSize(211, 37));
        pbemployee->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/employee_2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon8.addFile(QString::fromUtf8(":/icons/employee_2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbemployee->setIcon(icon8);
        pbemployee->setIconSize(QSize(110, 140));
        pbemployee->setCheckable(true);
        pbemployee->setAutoRepeatDelay(300);

        verticalLayout->addWidget(pbemployee);

        pbfabrication = new QPushButton(icon_text_widget);
        pbfabrication->setObjectName("pbfabrication");
        pbfabrication->setMaximumSize(QSize(211, 37));
        pbfabrication->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/fabrication_2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon9.addFile(QString::fromUtf8(":/icons/fabrication_2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbfabrication->setIcon(icon9);
        pbfabrication->setIconSize(QSize(100, 120));
        pbfabrication->setCheckable(true);

        verticalLayout->addWidget(pbfabrication);

        pbmodele = new QPushButton(icon_text_widget);
        pbmodele->setObjectName("pbmodele");
        pbmodele->setMaximumSize(QSize(211, 37));
        pbmodele->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/model_2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon10.addFile(QString::fromUtf8(":/icons/model_2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbmodele->setIcon(icon10);
        pbmodele->setIconSize(QSize(100, 130));
        pbmodele->setCheckable(true);
        pbmodele->setAutoRepeatInterval(100);

        verticalLayout->addWidget(pbmodele);

        pbpieces = new QPushButton(icon_text_widget);
        pbpieces->setObjectName("pbpieces");
        pbpieces->setMaximumSize(QSize(211, 37));
        pbpieces->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/piece_2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon11.addFile(QString::fromUtf8(":/icons/piece_2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbpieces->setIcon(icon11);
        pbpieces->setIconSize(QSize(120, 120));
        pbpieces->setCheckable(true);

        verticalLayout->addWidget(pbpieces);


        verticalLayout_6->addLayout(verticalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 198, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        pbsettings = new QPushButton(icon_text_widget);
        pbsettings->setObjectName("pbsettings");
        pbsettings->setMaximumSize(QSize(211, 31));
        pbsettings->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/settings2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon12.addFile(QString::fromUtf8(":/icons/settings2_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pbsettings->setIcon(icon12);
        pbsettings->setIconSize(QSize(100, 40));
        pbsettings->setCheckable(true);
        pbsettings->setAutoExclusive(false);

        verticalLayout_4->addWidget(pbsettings);

        pushButton_10 = new QPushButton(icon_text_widget);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setMaximumSize(QSize(211, 31));
        pushButton_10->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/signout2.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon13.addFile(QString::fromUtf8(":/icons/signout2_blue.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_10->setIcon(icon13);
        pushButton_10->setIconSize(QSize(100, 40));
        pushButton_10->setCheckable(true);
        pushButton_10->setAutoExclusive(false);

        verticalLayout_4->addWidget(pushButton_10);


        verticalLayout_6->addLayout(verticalLayout_4);

        verticalSpacer_5 = new QSpacerItem(20, 108, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_5);


        gridLayout_4->addWidget(icon_text_widget, 0, 1, 2, 1);

        header_widget = new QWidget(layoutWidget);
        header_widget->setObjectName("header_widget");
        header_widget->setStyleSheet(QString::fromUtf8("QWidget{\n"
"background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0c0c1d, stop:1 #111128);\n"
"border-bottom: 1px solid rgba(99,102,241,0.1);\n"
"}"));
        gridLayout_3 = new QGridLayout(header_widget);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_5 = new QLabel(header_widget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 1, 1, 1, 1);

        pushButton_17 = new QPushButton(header_widget);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"padding-left: 0px;\n"
"border: none;\n"
"background-color: transparent;\n"
"border-radius: 8px;\n"
"margin: 2px 6px;\n"
"padding: 6px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgba(255,255,255,0.08);\n"
"border-radius: 8px;\n"
"}\n"
"QPushButton:checked{\n"
"background-color: rgba(99,102,241,0.15);\n"
"border-left: 3px solid #6366f1;\n"
"border-radius: 8px;\n"
"}"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/menu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        icon14.addFile(QString::fromUtf8(":/icons/menu_blu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        pushButton_17->setIcon(icon14);
        pushButton_17->setCheckable(true);
        pushButton_17->setAutoExclusive(true);

        gridLayout->addWidget(pushButton_17, 0, 0, 2, 1);

        label_4 = new QLabel(header_widget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        Profil_photo = new QPushButton(header_widget);
        Profil_photo->setObjectName("Profil_photo");
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/275386664_2053585921482874_5027067967595041409_n-(1).png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Profil_photo->setIcon(icon15);
        Profil_photo->setIconSize(QSize(100, 55));

        gridLayout_2->addWidget(Profil_photo, 0, 1, 1, 1);

        lineEdit = new QLineEdit(header_widget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgba(255,255,255,0.05);\n"
"color: rgba(255,255,255,0.9);\n"
"border: 1px solid rgba(99,102,241,0.2);\n"
"border-radius: 10px;\n"
"padding: 10px 16px;\n"
"font-family: 'Segoe UI', 'Inter', sans-serif;\n"
"font-size: 13px;\n"
"selection-background-color: rgba(99,102,241,0.4);"));

        gridLayout_2->addWidget(lineEdit, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(582, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 1, 1, 1);


        gridLayout_4->addWidget(header_widget, 0, 2, 1, 1);

        stackedWidget = new QStackedWidget(layoutWidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0e0e20, stop:0.5 #121230, stop:1 #16163a);"));
        Pemployee = new QWidget();
        Pemployee->setObjectName("Pemployee");
        c_Pemp = new QWidget(Pemployee);
        c_Pemp->setObjectName("c_Pemp");
        c_Pemp->setGeometry(QRect(0, 0, 1201, 731));
        c_Pemp->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}QComboBox:hover{border:1px solid #6366f1;}QComboBox::drop-down{border:none;}QDateEdit,QSpinBox,QDoubleSpinBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-rad"
                        "ius:8px;padding:7px 10px;color:white;}QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}QTableView{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}QTableView::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}QScrollBar:vertical{border:none;background-color:transparent;width:8px;}QScrollBar::handle:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #818cf8,stop:1 #4f46e5);min-height:24px;border-"
                        "radius:4px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{border:none;background:none;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}QProgressBar{border:1px solid rgba(99,102,241,0.18);border-radius:8px;background:rgba(255,255,255,0.04);text-align:center;color:white;min-height:12px;}QProgressBar::chunk{border-radius:7px;background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #22c55e);}QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}"));
        t_emp = new QLabel(c_Pemp);
        t_emp->setObjectName("t_emp");
        t_emp->setGeometry(QRect(30, 10, 400, 35));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setWeight(QFont::Medium);
        t_emp->setFont(font);
        t_emp->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        tabWidget_emp = new QTabWidget(c_Pemp);
        tabWidget_emp->setObjectName("tabWidget_emp");
        tabWidget_emp->setGeometry(QRect(0, 50, 1181, 671));
        tab_emp_list = new QWidget();
        tab_emp_list->setObjectName("tab_emp_list");
        vl_emp_list = new QVBoxLayout(tab_emp_list);
        vl_emp_list->setSpacing(8);
        vl_emp_list->setObjectName("vl_emp_list");
        vl_emp_list->setContentsMargins(0, 0, 0, 0);
        hl_emp_top = new QHBoxLayout();
        hl_emp_top->setSpacing(4);
        hl_emp_top->setObjectName("hl_emp_top");
        search_emp = new QLineEdit(tab_emp_list);
        search_emp->setObjectName("search_emp");

        hl_emp_top->addWidget(search_emp);

        lb_filter_role = new QLabel(tab_emp_list);
        lb_filter_role->setObjectName("lb_filter_role");
        lb_filter_role->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_emp_top->addWidget(lb_filter_role);

        combo_filter_role = new QComboBox(tab_emp_list);
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->addItem(QString());
        combo_filter_role->setObjectName("combo_filter_role");

        hl_emp_top->addWidget(combo_filter_role);


        vl_emp_list->addLayout(hl_emp_top);

        hl_emp_action_top = new QHBoxLayout();
        hl_emp_action_top->setSpacing(4);
        hl_emp_action_top->setObjectName("hl_emp_action_top");
        btn_emp_action_export = new QPushButton(tab_emp_list);
        btn_emp_action_export->setObjectName("btn_emp_action_export");
        btn_emp_action_export->setVisible(false);

        hl_emp_action_top->addWidget(btn_emp_action_export);

        btn_emp_action_refresh = new QPushButton(tab_emp_list);
        btn_emp_action_refresh->setObjectName("btn_emp_action_refresh");

        hl_emp_action_top->addWidget(btn_emp_action_refresh);

        hs_emp_action_top_right = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_emp_action_top->addItem(hs_emp_action_top_right);


        vl_emp_list->addLayout(hl_emp_action_top);

        tv_emp = new QTableView(tab_emp_list);
        tv_emp->setObjectName("tv_emp");
        tv_emp->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_emp->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_emp->setShowGrid(false);
        tv_emp->setSortingEnabled(true);
        tv_emp->horizontalHeader()->setStretchLastSection(true);
        tv_emp->verticalHeader()->setVisible(false);
        tv_emp->verticalHeader()->setDefaultSectionSize(45);

        vl_emp_list->addWidget(tv_emp);

        hl_emp_pagination = new QHBoxLayout();
        hl_emp_pagination->setObjectName("hl_emp_pagination");
        hs_emp_pag_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_emp_pagination->addItem(hs_emp_pag_left);

        btn_emp_page_prev = new QPushButton(tab_emp_list);
        btn_emp_page_prev->setObjectName("btn_emp_page_prev");
        btn_emp_page_prev->setMinimumSize(QSize(28, 28));
        btn_emp_page_prev->setMaximumSize(QSize(28, 28));
        btn_emp_page_prev->setStyleSheet(QString::fromUtf8("QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-size:11px;font-weight:bold;padding:0px;margin:0px;}QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}QPushButton:disabled{color:rgba(255,255,255,0.2);border-color:rgba(255,255,255,0.05);}"));

        hl_emp_pagination->addWidget(btn_emp_page_prev);

        emp_page_nums_container = new QWidget(tab_emp_list);
        emp_page_nums_container->setObjectName("emp_page_nums_container");
        emp_page_nums_container->setMinimumSize(QSize(0, 36));
        emp_page_nums_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_emp_pagination->addWidget(emp_page_nums_container);

        btn_emp_page_next = new QPushButton(tab_emp_list);
        btn_emp_page_next->setObjectName("btn_emp_page_next");
        btn_emp_page_next->setMinimumSize(QSize(28, 28));
        btn_emp_page_next->setMaximumSize(QSize(28, 28));
        btn_emp_page_next->setStyleSheet(QString::fromUtf8("QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-size:11px;font-weight:bold;padding:0px;margin:0px;}QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}QPushButton:disabled{color:rgba(255,255,255,0.2);border-color:rgba(255,255,255,0.05);}"));

        hl_emp_pagination->addWidget(btn_emp_page_next);

        lb_emp_pagination = new QLabel(tab_emp_list);
        lb_emp_pagination->setObjectName("lb_emp_pagination");
        lb_emp_pagination->setVisible(false);
        lb_emp_pagination->setStyleSheet(QString::fromUtf8("background:transparent;color:#cbd5e1;font-weight:600;"));

        hl_emp_pagination->addWidget(lb_emp_pagination);

        hs_emp_pag_right = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_emp_pagination->addItem(hs_emp_pag_right);


        vl_emp_list->addLayout(hl_emp_pagination);

        hl_emp_actions = new QHBoxLayout();
        hl_emp_actions->setObjectName("hl_emp_actions");
        hs_emp_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_emp_actions->addItem(hs_emp_left);

        btn_emp_list_modify = new QPushButton(tab_emp_list);
        btn_emp_list_modify->setObjectName("btn_emp_list_modify");
        btn_emp_list_modify->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #2563eb,stop:1 #3b82f6);padding:10px 30px;"));

        hl_emp_actions->addWidget(btn_emp_list_modify);

        btn_emp_list_delete = new QPushButton(tab_emp_list);
        btn_emp_list_delete->setObjectName("btn_emp_list_delete");
        btn_emp_list_delete->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:10px 30px;"));

        hl_emp_actions->addWidget(btn_emp_list_delete);


        vl_emp_list->addLayout(hl_emp_actions);

        tabWidget_emp->addTab(tab_emp_list, QString());
        tab_emp_form = new QWidget();
        tab_emp_form->setObjectName("tab_emp_form");
        vl_emp_form = new QVBoxLayout(tab_emp_form);
        vl_emp_form->setObjectName("vl_emp_form");
        vl_emp_form->setContentsMargins(20, 20, 20, 20);
        gf_emp = new QGroupBox(tab_emp_form);
        gf_emp->setObjectName("gf_emp");
        fl_emp = new QGridLayout(gf_emp);
        fl_emp->setObjectName("fl_emp");
        fl_emp->setHorizontalSpacing(15);
        fl_emp->setVerticalSpacing(8);
        le1 = new QLabel(gf_emp);
        le1->setObjectName("le1");

        fl_emp->addWidget(le1, 1, 0, 1, 1);

        emp_cin = new QLineEdit(gf_emp);
        emp_cin->setObjectName("emp_cin");
        emp_cin->setMaxLength(20);

        fl_emp->addWidget(emp_cin, 1, 1, 1, 1);

        le2 = new QLabel(gf_emp);
        le2->setObjectName("le2");

        fl_emp->addWidget(le2, 2, 0, 1, 1);

        emp_mail = new QLineEdit(gf_emp);
        emp_mail->setObjectName("emp_mail");
        emp_mail->setMaxLength(100);

        fl_emp->addWidget(emp_mail, 2, 1, 1, 1);

        le3 = new QLabel(gf_emp);
        le3->setObjectName("le3");

        fl_emp->addWidget(le3, 3, 0, 1, 1);

        emp_password = new QLineEdit(gf_emp);
        emp_password->setObjectName("emp_password");
        emp_password->setMaxLength(255);
        emp_password->setEchoMode(QLineEdit::EchoMode::Password);

        fl_emp->addWidget(emp_password, 3, 1, 1, 1);

        le4 = new QLabel(gf_emp);
        le4->setObjectName("le4");

        fl_emp->addWidget(le4, 4, 0, 1, 1);

        emp_nom = new QLineEdit(gf_emp);
        emp_nom->setObjectName("emp_nom");
        emp_nom->setMaxLength(50);

        fl_emp->addWidget(emp_nom, 4, 1, 1, 1);

        le5 = new QLabel(gf_emp);
        le5->setObjectName("le5");

        fl_emp->addWidget(le5, 5, 0, 1, 1);

        emp_prenom = new QLineEdit(gf_emp);
        emp_prenom->setObjectName("emp_prenom");
        emp_prenom->setMaxLength(50);

        fl_emp->addWidget(emp_prenom, 5, 1, 1, 1);

        le6 = new QLabel(gf_emp);
        le6->setObjectName("le6");

        fl_emp->addWidget(le6, 6, 0, 1, 1);

        emp_date_naissance = new QDateEdit(gf_emp);
        emp_date_naissance->setObjectName("emp_date_naissance");
        emp_date_naissance->setCalendarPopup(true);

        fl_emp->addWidget(emp_date_naissance, 6, 1, 1, 1);

        le7 = new QLabel(gf_emp);
        le7->setObjectName("le7");

        fl_emp->addWidget(le7, 7, 0, 1, 1);

        emp_role = new QComboBox(gf_emp);
        emp_role->addItem(QString());
        emp_role->addItem(QString());
        emp_role->addItem(QString());
        emp_role->addItem(QString());
        emp_role->addItem(QString());
        emp_role->addItem(QString());
        emp_role->setObjectName("emp_role");

        fl_emp->addWidget(emp_role, 7, 1, 1, 1);

        le10 = new QLabel(gf_emp);
        le10->setObjectName("le10");

        fl_emp->addWidget(le10, 10, 0, 1, 1);

        hl_emp_photo = new QHBoxLayout();
        hl_emp_photo->setObjectName("hl_emp_photo");
        emp_face_id = new QLineEdit(gf_emp);
        emp_face_id->setObjectName("emp_face_id");

        hl_emp_photo->addWidget(emp_face_id);

        btn_emp_photo = new QPushButton(gf_emp);
        btn_emp_photo->setObjectName("btn_emp_photo");

        hl_emp_photo->addWidget(btn_emp_photo);


        fl_emp->addLayout(hl_emp_photo, 10, 1, 1, 1);


        vl_emp_form->addWidget(gf_emp);

        bl_emp = new QHBoxLayout();
        bl_emp->setObjectName("bl_emp");
        btn_emp_add = new QPushButton(tab_emp_form);
        btn_emp_add->setObjectName("btn_emp_add");

        bl_emp->addWidget(btn_emp_add);

        btn_emp_edit = new QPushButton(tab_emp_form);
        btn_emp_edit->setObjectName("btn_emp_edit");

        bl_emp->addWidget(btn_emp_edit);

        btn_emp_del = new QPushButton(tab_emp_form);
        btn_emp_del->setObjectName("btn_emp_del");
        btn_emp_del->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);"));

        bl_emp->addWidget(btn_emp_del);

        btn_emp_clr = new QPushButton(tab_emp_form);
        btn_emp_clr->setObjectName("btn_emp_clr");
        btn_emp_clr->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        bl_emp->addWidget(btn_emp_clr);


        vl_emp_form->addLayout(bl_emp);

        verticalSpacer_emp = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_emp_form->addItem(verticalSpacer_emp);

        tabWidget_emp->addTab(tab_emp_form, QString());
        tab_emp_affectation = new QWidget();
        tab_emp_affectation->setObjectName("tab_emp_affectation");
        vl_emp_affect = new QVBoxLayout(tab_emp_affectation);
        vl_emp_affect->setObjectName("vl_emp_affect");
        vl_emp_affect->setContentsMargins(15, 15, 15, 15);
        hl_affect_panels = new QHBoxLayout();
        hl_affect_panels->setSpacing(15);
        hl_affect_panels->setObjectName("hl_affect_panels");
        gb_affect_emp = new QGroupBox(tab_emp_affectation);
        gb_affect_emp->setObjectName("gb_affect_emp");
        vl_affect_emp_inner = new QVBoxLayout(gb_affect_emp);
        vl_affect_emp_inner->setObjectName("vl_affect_emp_inner");
        search_affect_emp = new QLineEdit(gb_affect_emp);
        search_affect_emp->setObjectName("search_affect_emp");

        vl_affect_emp_inner->addWidget(search_affect_emp);

        tv_affect_emp = new QTableView(gb_affect_emp);
        tv_affect_emp->setObjectName("tv_affect_emp");
        tv_affect_emp->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_affect_emp->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_affect_emp->setShowGrid(false);
        tv_affect_emp->setSortingEnabled(true);
        tv_affect_emp->horizontalHeader()->setStretchLastSection(true);
        tv_affect_emp->verticalHeader()->setVisible(false);
        tv_affect_emp->verticalHeader()->setDefaultSectionSize(40);

        vl_affect_emp_inner->addWidget(tv_affect_emp);


        hl_affect_panels->addWidget(gb_affect_emp);

        gb_affect_fab = new QGroupBox(tab_emp_affectation);
        gb_affect_fab->setObjectName("gb_affect_fab");
        vl_affect_fab_inner = new QVBoxLayout(gb_affect_fab);
        vl_affect_fab_inner->setObjectName("vl_affect_fab_inner");
        search_affect_fab = new QLineEdit(gb_affect_fab);
        search_affect_fab->setObjectName("search_affect_fab");

        vl_affect_fab_inner->addWidget(search_affect_fab);

        tv_affect_fab = new QTableView(gb_affect_fab);
        tv_affect_fab->setObjectName("tv_affect_fab");
        tv_affect_fab->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_affect_fab->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_affect_fab->setShowGrid(false);
        tv_affect_fab->setSortingEnabled(true);
        tv_affect_fab->horizontalHeader()->setStretchLastSection(true);
        tv_affect_fab->verticalHeader()->setVisible(false);
        tv_affect_fab->verticalHeader()->setDefaultSectionSize(40);

        vl_affect_fab_inner->addWidget(tv_affect_fab);


        hl_affect_panels->addWidget(gb_affect_fab);


        vl_emp_affect->addLayout(hl_affect_panels);

        hl_affect_actions = new QHBoxLayout();
        hl_affect_actions->setObjectName("hl_affect_actions");
        hs_affect_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_affect_actions->addItem(hs_affect_left);

        btn_emp_affect = new QPushButton(tab_emp_affectation);
        btn_emp_affect->setObjectName("btn_emp_affect");
        btn_emp_affect->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #059669,stop:1 #10b981);padding:10px 30px;"));

        hl_affect_actions->addWidget(btn_emp_affect);

        btn_emp_unaffect = new QPushButton(tab_emp_affectation);
        btn_emp_unaffect->setObjectName("btn_emp_unaffect");
        btn_emp_unaffect->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);padding:10px 30px;"));

        hl_affect_actions->addWidget(btn_emp_unaffect);

        hs_affect_right = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_affect_actions->addItem(hs_affect_right);


        vl_emp_affect->addLayout(hl_affect_actions);

        tabWidget_emp->addTab(tab_emp_affectation, QString());
        tab_emp_stats = new QWidget();
        tab_emp_stats->setObjectName("tab_emp_stats");
        vl_emp_stats = new QVBoxLayout(tab_emp_stats);
        vl_emp_stats->setObjectName("vl_emp_stats");
        vl_emp_stats->setContentsMargins(20, 20, 20, 20);
        hl_emp_stats_cards = new QHBoxLayout();
        hl_emp_stats_cards->setObjectName("hl_emp_stats_cards");
        gb_emp_stats_total = new QGroupBox(tab_emp_stats);
        gb_emp_stats_total->setObjectName("gb_emp_stats_total");
        vl_emp_stats_total = new QVBoxLayout(gb_emp_stats_total);
        vl_emp_stats_total->setObjectName("vl_emp_stats_total");
        lb_emp_stats_total_title = new QLabel(gb_emp_stats_total);
        lb_emp_stats_total_title->setObjectName("lb_emp_stats_total_title");
        lb_emp_stats_total_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_emp_stats_total->addWidget(lb_emp_stats_total_title);

        lb_emp_stats_total_value = new QLabel(gb_emp_stats_total);
        lb_emp_stats_total_value->setObjectName("lb_emp_stats_total_value");
        lb_emp_stats_total_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:28px;font-weight:700;"));

        vl_emp_stats_total->addWidget(lb_emp_stats_total_value);

        lb_emp_stats_total_hint = new QLabel(gb_emp_stats_total);
        lb_emp_stats_total_hint->setObjectName("lb_emp_stats_total_hint");
        lb_emp_stats_total_hint->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.65);background:transparent;"));

        vl_emp_stats_total->addWidget(lb_emp_stats_total_hint);


        hl_emp_stats_cards->addWidget(gb_emp_stats_total);

        gb_emp_stats_affect = new QGroupBox(tab_emp_stats);
        gb_emp_stats_affect->setObjectName("gb_emp_stats_affect");
        vl_emp_stats_affect = new QVBoxLayout(gb_emp_stats_affect);
        vl_emp_stats_affect->setObjectName("vl_emp_stats_affect");
        lb_emp_stats_affect_title = new QLabel(gb_emp_stats_affect);
        lb_emp_stats_affect_title->setObjectName("lb_emp_stats_affect_title");
        lb_emp_stats_affect_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_emp_stats_affect->addWidget(lb_emp_stats_affect_title);

        lb_emp_stats_affected_value = new QLabel(gb_emp_stats_affect);
        lb_emp_stats_affected_value->setObjectName("lb_emp_stats_affected_value");
        lb_emp_stats_affected_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:28px;font-weight:700;"));

        vl_emp_stats_affect->addWidget(lb_emp_stats_affected_value);

        pb_emp_stats_affected = new QProgressBar(gb_emp_stats_affect);
        pb_emp_stats_affected->setObjectName("pb_emp_stats_affected");
        pb_emp_stats_affected->setValue(0);

        vl_emp_stats_affect->addWidget(pb_emp_stats_affected);


        hl_emp_stats_cards->addWidget(gb_emp_stats_affect);

        gb_emp_stats_toprole = new QGroupBox(tab_emp_stats);
        gb_emp_stats_toprole->setObjectName("gb_emp_stats_toprole");
        vl_emp_stats_toprole = new QVBoxLayout(gb_emp_stats_toprole);
        vl_emp_stats_toprole->setObjectName("vl_emp_stats_toprole");
        lb_emp_stats_toprole_title = new QLabel(gb_emp_stats_toprole);
        lb_emp_stats_toprole_title->setObjectName("lb_emp_stats_toprole_title");
        lb_emp_stats_toprole_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_emp_stats_toprole->addWidget(lb_emp_stats_toprole_title);

        lb_emp_stats_toprole_value = new QLabel(gb_emp_stats_toprole);
        lb_emp_stats_toprole_value->setObjectName("lb_emp_stats_toprole_value");
        lb_emp_stats_toprole_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:20px;font-weight:700;"));

        vl_emp_stats_toprole->addWidget(lb_emp_stats_toprole_value);

        lb_emp_stats_toprole_hint = new QLabel(gb_emp_stats_toprole);
        lb_emp_stats_toprole_hint->setObjectName("lb_emp_stats_toprole_hint");
        lb_emp_stats_toprole_hint->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.65);background:transparent;"));

        vl_emp_stats_toprole->addWidget(lb_emp_stats_toprole_hint);


        hl_emp_stats_cards->addWidget(gb_emp_stats_toprole);


        vl_emp_stats->addLayout(hl_emp_stats_cards);

        hl_emp_stats_body = new QHBoxLayout();
        hl_emp_stats_body->setObjectName("hl_emp_stats_body");
        gb_emp_stats_roles = new QGroupBox(tab_emp_stats);
        gb_emp_stats_roles->setObjectName("gb_emp_stats_roles");
        vl_emp_stats_roles = new QVBoxLayout(gb_emp_stats_roles);
        vl_emp_stats_roles->setObjectName("vl_emp_stats_roles");
        lb_emp_stats_role_1 = new QLabel(gb_emp_stats_roles);
        lb_emp_stats_role_1->setObjectName("lb_emp_stats_role_1");

        vl_emp_stats_roles->addWidget(lb_emp_stats_role_1);

        pb_emp_stats_role_1 = new QProgressBar(gb_emp_stats_roles);
        pb_emp_stats_role_1->setObjectName("pb_emp_stats_role_1");
        pb_emp_stats_role_1->setValue(0);

        vl_emp_stats_roles->addWidget(pb_emp_stats_role_1);

        lb_emp_stats_role_2 = new QLabel(gb_emp_stats_roles);
        lb_emp_stats_role_2->setObjectName("lb_emp_stats_role_2");

        vl_emp_stats_roles->addWidget(lb_emp_stats_role_2);

        pb_emp_stats_role_2 = new QProgressBar(gb_emp_stats_roles);
        pb_emp_stats_role_2->setObjectName("pb_emp_stats_role_2");
        pb_emp_stats_role_2->setValue(0);

        vl_emp_stats_roles->addWidget(pb_emp_stats_role_2);

        lb_emp_stats_role_3 = new QLabel(gb_emp_stats_roles);
        lb_emp_stats_role_3->setObjectName("lb_emp_stats_role_3");

        vl_emp_stats_roles->addWidget(lb_emp_stats_role_3);

        pb_emp_stats_role_3 = new QProgressBar(gb_emp_stats_roles);
        pb_emp_stats_role_3->setObjectName("pb_emp_stats_role_3");
        pb_emp_stats_role_3->setValue(0);

        vl_emp_stats_roles->addWidget(pb_emp_stats_role_3);

        emp_stats_role_chart_container = new QWidget(gb_emp_stats_roles);
        emp_stats_role_chart_container->setObjectName("emp_stats_role_chart_container");
        emp_stats_role_chart_container->setMinimumSize(QSize(0, 180));
        emp_stats_role_chart_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        vl_emp_stats_roles->addWidget(emp_stats_role_chart_container);

        vs_emp_stats_roles = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_emp_stats_roles->addItem(vs_emp_stats_roles);


        hl_emp_stats_body->addWidget(gb_emp_stats_roles);

        gb_emp_stats_pipeline = new QGroupBox(tab_emp_stats);
        gb_emp_stats_pipeline->setObjectName("gb_emp_stats_pipeline");
        vl_emp_stats_pipeline = new QVBoxLayout(gb_emp_stats_pipeline);
        vl_emp_stats_pipeline->setObjectName("vl_emp_stats_pipeline");
        lb_emp_stats_pipeline_title = new QLabel(gb_emp_stats_pipeline);
        lb_emp_stats_pipeline_title->setObjectName("lb_emp_stats_pipeline_title");
        lb_emp_stats_pipeline_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_emp_stats_pipeline->addWidget(lb_emp_stats_pipeline_title);

        emp_stats_fab_chart_container = new QWidget(gb_emp_stats_pipeline);
        emp_stats_fab_chart_container->setObjectName("emp_stats_fab_chart_container");
        emp_stats_fab_chart_container->setMinimumSize(QSize(0, 180));
        emp_stats_fab_chart_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        vl_emp_stats_pipeline->addWidget(emp_stats_fab_chart_container);

        txt_emp_stats_pipeline = new QTextEdit(gb_emp_stats_pipeline);
        txt_emp_stats_pipeline->setObjectName("txt_emp_stats_pipeline");
        txt_emp_stats_pipeline->setMaximumSize(QSize(16777215, 220));
        txt_emp_stats_pipeline->setReadOnly(true);

        vl_emp_stats_pipeline->addWidget(txt_emp_stats_pipeline);

        btn_emp_stats_refresh = new QPushButton(gb_emp_stats_pipeline);
        btn_emp_stats_refresh->setObjectName("btn_emp_stats_refresh");

        vl_emp_stats_pipeline->addWidget(btn_emp_stats_refresh);

        vs_emp_stats_pipeline = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_emp_stats_pipeline->addItem(vs_emp_stats_pipeline);


        hl_emp_stats_body->addWidget(gb_emp_stats_pipeline);


        vl_emp_stats->addLayout(hl_emp_stats_body);

        tabWidget_emp->addTab(tab_emp_stats, QString());
        tab_emp_knn = new QWidget();
        tab_emp_knn->setObjectName("tab_emp_knn");
        vl_emp_knn = new QVBoxLayout(tab_emp_knn);
        vl_emp_knn->setObjectName("vl_emp_knn");
        vl_emp_knn->setContentsMargins(20, 20, 20, 20);
        gb_emp_knn_inputs = new QGroupBox(tab_emp_knn);
        gb_emp_knn_inputs->setObjectName("gb_emp_knn_inputs");
        gl_emp_knn_inputs = new QGridLayout(gb_emp_knn_inputs);
        gl_emp_knn_inputs->setObjectName("gl_emp_knn_inputs");
        lb_emp_knn_task_x = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_task_x->setObjectName("lb_emp_knn_task_x");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_task_x, 0, 0, 1, 1);

        emp_knn_task_x = new QDoubleSpinBox(gb_emp_knn_inputs);
        emp_knn_task_x->setObjectName("emp_knn_task_x");
        emp_knn_task_x->setDecimals(2);
        emp_knn_task_x->setMinimum(-100.000000000000000);
        emp_knn_task_x->setMaximum(100.000000000000000);

        gl_emp_knn_inputs->addWidget(emp_knn_task_x, 0, 1, 1, 1);

        lb_emp_knn_task_y = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_task_y->setObjectName("lb_emp_knn_task_y");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_task_y, 1, 0, 1, 1);

        emp_knn_task_y = new QDoubleSpinBox(gb_emp_knn_inputs);
        emp_knn_task_y->setObjectName("emp_knn_task_y");
        emp_knn_task_y->setDecimals(2);
        emp_knn_task_y->setMinimum(-100.000000000000000);
        emp_knn_task_y->setMaximum(100.000000000000000);

        gl_emp_knn_inputs->addWidget(emp_knn_task_y, 1, 1, 1, 1);

        lb_emp_knn_k = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_k->setObjectName("lb_emp_knn_k");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_k, 1, 2, 1, 1);

        emp_knn_k = new QSpinBox(gb_emp_knn_inputs);
        emp_knn_k->setObjectName("emp_knn_k");
        emp_knn_k->setMinimum(1);
        emp_knn_k->setMaximum(25);
        emp_knn_k->setValue(5);

        gl_emp_knn_inputs->addWidget(emp_knn_k, 1, 3, 1, 1);

        lb_emp_knn_role = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_role->setObjectName("lb_emp_knn_role");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_role, 8, 0, 1, 1);

        emp_knn_role = new QComboBox(gb_emp_knn_inputs);
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->addItem(QString());
        emp_knn_role->setObjectName("emp_knn_role");

        gl_emp_knn_inputs->addWidget(emp_knn_role, 8, 1, 1, 1);

        lb_emp_knn_source_title = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_source_title->setObjectName("lb_emp_knn_source_title");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_source_title, 2, 0, 1, 4);

        lb_emp_knn_source_type = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_source_type->setObjectName("lb_emp_knn_source_type");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_source_type, 3, 0, 1, 1);

        emp_knn_source_type = new QComboBox(gb_emp_knn_inputs);
        emp_knn_source_type->addItem(QString());
        emp_knn_source_type->addItem(QString());
        emp_knn_source_type->addItem(QString());
        emp_knn_source_type->setObjectName("emp_knn_source_type");

        gl_emp_knn_inputs->addWidget(emp_knn_source_type, 3, 1, 1, 1);

        lb_emp_knn_source_id = new QLabel(gb_emp_knn_inputs);
        lb_emp_knn_source_id->setObjectName("lb_emp_knn_source_id");

        gl_emp_knn_inputs->addWidget(lb_emp_knn_source_id, 3, 2, 1, 1);

        emp_knn_source_id = new QSpinBox(gb_emp_knn_inputs);
        emp_knn_source_id->setObjectName("emp_knn_source_id");
        emp_knn_source_id->setMinimum(0);
        emp_knn_source_id->setMaximum(999999);

        gl_emp_knn_inputs->addWidget(emp_knn_source_id, 3, 3, 1, 1);

        btn_emp_knn_run = new QPushButton(gb_emp_knn_inputs);
        btn_emp_knn_run->setObjectName("btn_emp_knn_run");

        gl_emp_knn_inputs->addWidget(btn_emp_knn_run, 4, 0, 1, 4);


        vl_emp_knn->addWidget(gb_emp_knn_inputs);

        lb_emp_knn_summary = new QLabel(tab_emp_knn);
        lb_emp_knn_summary->setObjectName("lb_emp_knn_summary");
        lb_emp_knn_summary->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_emp_knn->addWidget(lb_emp_knn_summary);

        emp_knn_chart_container = new QFrame(tab_emp_knn);
        emp_knn_chart_container->setObjectName("emp_knn_chart_container");
        emp_knn_chart_container->setMinimumSize(QSize(0, 280));
        emp_knn_chart_container->setStyleSheet(QString::fromUtf8("background:#ffffff;border:1px solid #dbe3ea;border-radius:10px;"));
        emp_knn_chart_container->setFrameShape(QFrame::Shape::StyledPanel);
        emp_knn_chart_container->setFrameShadow(QFrame::Shadow::Raised);

        vl_emp_knn->addWidget(emp_knn_chart_container);

        tv_emp_knn = new QTableView(tab_emp_knn);
        tv_emp_knn->setObjectName("tv_emp_knn");
        tv_emp_knn->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_emp_knn->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_emp_knn->setShowGrid(false);
        tv_emp_knn->setSortingEnabled(true);
        tv_emp_knn->horizontalHeader()->setStretchLastSection(true);
        tv_emp_knn->verticalHeader()->setVisible(false);

        vl_emp_knn->addWidget(tv_emp_knn);

        tabWidget_emp->addTab(tab_emp_knn, QString());
        tab_emp_training = new QWidget();
        tab_emp_training->setObjectName("tab_emp_training");
        vl_emp_training = new QVBoxLayout(tab_emp_training);
        vl_emp_training->setObjectName("vl_emp_training");
        vl_emp_training->setContentsMargins(20, 20, 20, 20);
        gb_training_employee_select = new QGroupBox(tab_emp_training);
        gb_training_employee_select->setObjectName("gb_training_employee_select");
        hl_training_select = new QHBoxLayout(gb_training_employee_select);
        hl_training_select->setObjectName("hl_training_select");
        combo_training_employee = new QComboBox(gb_training_employee_select);
        combo_training_employee->setObjectName("combo_training_employee");

        hl_training_select->addWidget(combo_training_employee);

        btn_training_analyze = new QPushButton(gb_training_employee_select);
        btn_training_analyze->setObjectName("btn_training_analyze");

        hl_training_select->addWidget(btn_training_analyze);

        btn_training_refresh = new QPushButton(gb_training_employee_select);
        btn_training_refresh->setObjectName("btn_training_refresh");

        hl_training_select->addWidget(btn_training_refresh);

        horizontalSpacer_training = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_training_select->addItem(horizontalSpacer_training);


        vl_emp_training->addWidget(gb_training_employee_select);

        gb_training_profile = new QGroupBox(tab_emp_training);
        gb_training_profile->setObjectName("gb_training_profile");
        gl_training_profile = new QGridLayout(gb_training_profile);
        gl_training_profile->setObjectName("gl_training_profile");
        lb_training_role = new QLabel(gb_training_profile);
        lb_training_role->setObjectName("lb_training_role");

        gl_training_profile->addWidget(lb_training_role, 0, 0, 1, 1);

        lb_training_role_value = new QLabel(gb_training_profile);
        lb_training_role_value->setObjectName("lb_training_role_value");
        lb_training_role_value->setStyleSheet(QString::fromUtf8("font-weight:600;color:#818cf8;"));

        gl_training_profile->addWidget(lb_training_role_value, 0, 1, 1, 1);

        lb_training_level = new QLabel(gb_training_profile);
        lb_training_level->setObjectName("lb_training_level");

        gl_training_profile->addWidget(lb_training_level, 0, 2, 1, 1);

        lb_training_level_value = new QLabel(gb_training_profile);
        lb_training_level_value->setObjectName("lb_training_level_value");
        lb_training_level_value->setStyleSheet(QString::fromUtf8("font-weight:600;color:#22c55e;"));

        gl_training_profile->addWidget(lb_training_level_value, 0, 3, 1, 1);

        lb_training_seniority = new QLabel(gb_training_profile);
        lb_training_seniority->setObjectName("lb_training_seniority");

        gl_training_profile->addWidget(lb_training_seniority, 1, 0, 1, 1);

        lb_training_seniority_value = new QLabel(gb_training_profile);
        lb_training_seniority_value->setObjectName("lb_training_seniority_value");
        lb_training_seniority_value->setStyleSheet(QString::fromUtf8("font-weight:600;color:#f59e0b;"));

        gl_training_profile->addWidget(lb_training_seniority_value, 1, 1, 1, 1);

        lb_training_skills_count = new QLabel(gb_training_profile);
        lb_training_skills_count->setObjectName("lb_training_skills_count");

        gl_training_profile->addWidget(lb_training_skills_count, 1, 2, 1, 1);

        lb_training_skills_count_value = new QLabel(gb_training_profile);
        lb_training_skills_count_value->setObjectName("lb_training_skills_count_value");
        lb_training_skills_count_value->setStyleSheet(QString::fromUtf8("font-weight:600;color:#06b6d4;"));

        gl_training_profile->addWidget(lb_training_skills_count_value, 1, 3, 1, 1);


        vl_emp_training->addWidget(gb_training_profile);

        gb_training_recommendations = new QGroupBox(tab_emp_training);
        gb_training_recommendations->setObjectName("gb_training_recommendations");
        vl_training_recommendations = new QVBoxLayout(gb_training_recommendations);
        vl_training_recommendations->setObjectName("vl_training_recommendations");
        tw_training_recommendations = new QTableWidget(gb_training_recommendations);
        if (tw_training_recommendations->columnCount() < 5)
            tw_training_recommendations->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tw_training_recommendations->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tw_training_recommendations->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tw_training_recommendations->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tw_training_recommendations->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tw_training_recommendations->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tw_training_recommendations->setObjectName("tw_training_recommendations");
        tw_training_recommendations->setAlternatingRowColors(true);
        tw_training_recommendations->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tw_training_recommendations->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tw_training_recommendations->setShowGrid(false);
        tw_training_recommendations->setGridStyle(Qt::PenStyle::SolidLine);
        tw_training_recommendations->setRowCount(0);
        tw_training_recommendations->setColumnCount(5);

        vl_training_recommendations->addWidget(tw_training_recommendations);


        vl_emp_training->addWidget(gb_training_recommendations);

        gb_training_current = new QGroupBox(tab_emp_training);
        gb_training_current->setObjectName("gb_training_current");
        vl_training_completed = new QVBoxLayout(gb_training_current);
        vl_training_completed->setObjectName("vl_training_completed");
        tw_training_completed = new QTableWidget(gb_training_current);
        if (tw_training_completed->columnCount() < 4)
            tw_training_completed->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tw_training_completed->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tw_training_completed->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tw_training_completed->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tw_training_completed->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        tw_training_completed->setObjectName("tw_training_completed");
        tw_training_completed->setAlternatingRowColors(true);
        tw_training_completed->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tw_training_completed->setShowGrid(false);
        tw_training_completed->setRowCount(0);
        tw_training_completed->setColumnCount(4);

        vl_training_completed->addWidget(tw_training_completed);


        vl_emp_training->addWidget(gb_training_current);

        verticalSpacer_training = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_emp_training->addItem(verticalSpacer_training);

        tabWidget_emp->addTab(tab_emp_training, QString());
        stackedWidget->addWidget(Pemployee);
        Pbois = new QWidget();
        Pbois->setObjectName("Pbois");
        c_Pbois = new QWidget(Pbois);
        c_Pbois->setObjectName("c_Pbois");
        c_Pbois->setGeometry(QRect(0, 0, 1201, 731));
        c_Pbois->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}QComboBox:hover{border:1px solid #6366f1;}QComboBox::drop-down{border:none;}QDateEdit,QSpinBox,QDoubleSpinBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-rad"
                        "ius:8px;padding:7px 10px;color:white;}QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}QTableView{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}QTableView::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}QScrollBar:vertical{border:none;background-color:transparent;width:8px;}QScrollBar::handle:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #818cf8,stop:1 #4f46e5);min-height:24px;border-"
                        "radius:4px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{border:none;background:none;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}"));
        t_bois = new QLabel(c_Pbois);
        t_bois->setObjectName("t_bois");
        t_bois->setGeometry(QRect(30, 10, 400, 35));
        t_bois->setFont(font);
        t_bois->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        tabWidget_bois = new QTabWidget(c_Pbois);
        tabWidget_bois->setObjectName("tabWidget_bois");
        tabWidget_bois->setGeometry(QRect(10, 50, 1181, 671));
        tab_bois_list = new QWidget();
        tab_bois_list->setObjectName("tab_bois_list");
        vl_bois_list = new QVBoxLayout(tab_bois_list);
        vl_bois_list->setObjectName("vl_bois_list");
        vl_bois_list->setContentsMargins(20, 20, 20, 20);
        hl_bois_top = new QHBoxLayout();
        hl_bois_top->setObjectName("hl_bois_top");
        search_bois = new QLineEdit(tab_bois_list);
        search_bois->setObjectName("search_bois");

        hl_bois_top->addWidget(search_bois);

        lb_filter_fournisseur = new QLabel(tab_bois_list);
        lb_filter_fournisseur->setObjectName("lb_filter_fournisseur");
        lb_filter_fournisseur->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_bois_top->addWidget(lb_filter_fournisseur);

        combo_filter_fournisseur = new QComboBox(tab_bois_list);
        combo_filter_fournisseur->addItem(QString());
        combo_filter_fournisseur->addItem(QString());
        combo_filter_fournisseur->addItem(QString());
        combo_filter_fournisseur->addItem(QString());
        combo_filter_fournisseur->addItem(QString());
        combo_filter_fournisseur->setObjectName("combo_filter_fournisseur");

        hl_bois_top->addWidget(combo_filter_fournisseur);


        vl_bois_list->addLayout(hl_bois_top);

        hl_bois_sort = new QHBoxLayout();
        hl_bois_sort->setSpacing(15);
        hl_bois_sort->setObjectName("hl_bois_sort");
        lb_bois_sort = new QLabel(tab_bois_list);
        lb_bois_sort->setObjectName("lb_bois_sort");
        lb_bois_sort->setStyleSheet(QString::fromUtf8("color: #94a3b8; font-weight: bold; font-size: 11px;"));

        hl_bois_sort->addWidget(lb_bois_sort);

        cb_bois_sort_field = new QComboBox(tab_bois_list);
        cb_bois_sort_field->addItem(QString());
        cb_bois_sort_field->addItem(QString());
        cb_bois_sort_field->addItem(QString());
        cb_bois_sort_field->addItem(QString());
        cb_bois_sort_field->setObjectName("cb_bois_sort_field");
        cb_bois_sort_field->setMinimumSize(QSize(150, 30));
        cb_bois_sort_field->setStyleSheet(QString::fromUtf8("QComboBox { background: rgba(30,41,59,0.5); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; color: #f8fafc; padding: 5px 10px; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: url(:/icons/down_arrow.png); width: 12px; height: 12px; }"));

        hl_bois_sort->addWidget(cb_bois_sort_field);

        cb_bois_sort_order = new QComboBox(tab_bois_list);
        cb_bois_sort_order->addItem(QString());
        cb_bois_sort_order->addItem(QString());
        cb_bois_sort_order->setObjectName("cb_bois_sort_order");
        cb_bois_sort_order->setMinimumSize(QSize(120, 30));
        cb_bois_sort_order->setStyleSheet(QString::fromUtf8("QComboBox { background: rgba(30,41,59,0.5); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; color: #f8fafc; padding: 5px 10px; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: url(:/icons/down_arrow.png); width: 12px; height: 12px; }"));

        hl_bois_sort->addWidget(cb_bois_sort_order);

        hs_bois_sort = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_bois_sort->addItem(hs_bois_sort);


        vl_bois_list->addLayout(hl_bois_sort);

        tv_bois = new QTableView(tab_bois_list);
        tv_bois->setObjectName("tv_bois");
        tv_bois->setMaximumSize(QSize(16777215, 480));
        tv_bois->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tv_bois->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tv_bois->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked|QAbstractItemView::EditTrigger::EditKeyPressed);
        tv_bois->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_bois->setShowGrid(false);
        tv_bois->setSortingEnabled(true);
        tv_bois->horizontalHeader()->setStretchLastSection(true);
        tv_bois->verticalHeader()->setVisible(false);
        tv_bois->verticalHeader()->setDefaultSectionSize(45);

        vl_bois_list->addWidget(tv_bois);

        hl_bois_pagination = new QHBoxLayout();
        hl_bois_pagination->setObjectName("hl_bois_pagination");
        hs_bois_pag_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_bois_pagination->addItem(hs_bois_pag_left);

        btn_bois_prev = new QPushButton(tab_bois_list);
        btn_bois_prev->setObjectName("btn_bois_prev");
        btn_bois_prev->setMinimumSize(QSize(28, 28));
        btn_bois_prev->setMaximumSize(QSize(28, 28));
        btn_bois_prev->setStyleSheet(QString::fromUtf8("QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-size:11px;font-weight:bold;padding:0px;margin:0px;}QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}QPushButton:disabled{color:rgba(255,255,255,0.2);border-color:rgba(255,255,255,0.05);}"));

        hl_bois_pagination->addWidget(btn_bois_prev);

        bois_page_nums_container = new QWidget(tab_bois_list);
        bois_page_nums_container->setObjectName("bois_page_nums_container");
        bois_page_nums_container->setMinimumSize(QSize(0, 36));
        bois_page_nums_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_bois_pagination->addWidget(bois_page_nums_container);

        btn_bois_next = new QPushButton(tab_bois_list);
        btn_bois_next->setObjectName("btn_bois_next");
        btn_bois_next->setMinimumSize(QSize(28, 28));
        btn_bois_next->setMaximumSize(QSize(28, 28));
        btn_bois_next->setStyleSheet(QString::fromUtf8("QPushButton{background:rgba(99,102,241,0.15);color:#818cf8;border:1px solid rgba(99,102,241,0.3);border-radius:14px;font-size:11px;font-weight:bold;padding:0px;margin:0px;}QPushButton:hover{background:rgba(99,102,241,0.35);color:white;}QPushButton:disabled{color:rgba(255,255,255,0.2);border-color:rgba(255,255,255,0.05);}"));

        hl_bois_pagination->addWidget(btn_bois_next);

        lb_bois_pagination = new QLabel(tab_bois_list);
        lb_bois_pagination->setObjectName("lb_bois_pagination");
        lb_bois_pagination->setVisible(false);

        hl_bois_pagination->addWidget(lb_bois_pagination);

        hs_bois_pag_right = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_bois_pagination->addItem(hs_bois_pag_right);


        vl_bois_list->addLayout(hl_bois_pagination);

        hl_bois_actions = new QHBoxLayout();
        hl_bois_actions->setObjectName("hl_bois_actions");
        hs_bois_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_bois_actions->addItem(hs_bois_left);

        btn_bois_list_add = new QPushButton(tab_bois_list);
        btn_bois_list_add->setObjectName("btn_bois_list_add");
        btn_bois_list_add->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #2563eb,stop:1 #3b82f6);padding:8px 25px;"));

        hl_bois_actions->addWidget(btn_bois_list_add);

        btn_bois_list_delete = new QPushButton(tab_bois_list);
        btn_bois_list_delete->setObjectName("btn_bois_list_delete");
        btn_bois_list_delete->setVisible(false);
        btn_bois_list_delete->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);padding:8px 25px;"));

        hl_bois_actions->addWidget(btn_bois_list_delete);

        btn_bois_export_pdf = new QPushButton(tab_bois_list);
        btn_bois_export_pdf->setObjectName("btn_bois_export_pdf");
        btn_bois_export_pdf->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:8px 25px;"));

        hl_bois_actions->addWidget(btn_bois_export_pdf);


        vl_bois_list->addLayout(hl_bois_actions);

        tabWidget_bois->addTab(tab_bois_list, QString());
        tab_bois_stats = new QWidget();
        tab_bois_stats->setObjectName("tab_bois_stats");
        vl_bois_stats = new QVBoxLayout(tab_bois_stats);
        vl_bois_stats->setObjectName("vl_bois_stats");
        vl_bois_stats->setContentsMargins(20, 20, 20, 20);
        hl_bois_stats_top = new QHBoxLayout();
        hl_bois_stats_top->setObjectName("hl_bois_stats_top");
        lb_bois_stats_title = new QLabel(tab_bois_stats);
        lb_bois_stats_title->setObjectName("lb_bois_stats_title");
        lb_bois_stats_title->setStyleSheet(QString::fromUtf8("color:#818cf8;font-size:17px;font-weight:700;background:transparent;"));

        hl_bois_stats_top->addWidget(lb_bois_stats_title);

        hs_bois_stats_spacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_bois_stats_top->addItem(hs_bois_stats_spacer);

        btn_bois_stats_refresh = new QPushButton(tab_bois_stats);
        btn_bois_stats_refresh->setObjectName("btn_bois_stats_refresh");

        hl_bois_stats_top->addWidget(btn_bois_stats_refresh);


        vl_bois_stats->addLayout(hl_bois_stats_top);

        tab_bois_stats_charts = new QFrame(tab_bois_stats);
        tab_bois_stats_charts->setObjectName("tab_bois_stats_charts");
        tab_bois_stats_charts->setMinimumSize(QSize(0, 500));
        tab_bois_stats_charts->setStyleSheet(QString::fromUtf8("background:rgba(18,18,42,0.8);border:1px solid rgba(99,102,241,0.15);border-radius:12px;"));
        tab_bois_stats_charts->setFrameShape(QFrame::Shape::StyledPanel);
        tab_bois_stats_charts->setFrameShadow(QFrame::Shadow::Raised);

        vl_bois_stats->addWidget(tab_bois_stats_charts);

        tabWidget_bois->addTab(tab_bois_stats, QString());
        tab_bois_ai = new QWidget();
        tab_bois_ai->setObjectName("tab_bois_ai");
        vl_bois_ai = new QVBoxLayout(tab_bois_ai);
        vl_bois_ai->setObjectName("vl_bois_ai");
        vl_bois_ai->setContentsMargins(20, 20, 20, 20);
        gb_bois_recommend = new QGroupBox(tab_bois_ai);
        gb_bois_recommend->setObjectName("gb_bois_recommend");
        vl_bois_recommend = new QVBoxLayout(gb_bois_recommend);
        vl_bois_recommend->setObjectName("vl_bois_recommend");
        lb_bois_recommend_info = new QLabel(gb_bois_recommend);
        lb_bois_recommend_info->setObjectName("lb_bois_recommend_info");
        lb_bois_recommend_info->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.75);background:transparent;font-size:13px;"));
        lb_bois_recommend_info->setWordWrap(true);

        vl_bois_recommend->addWidget(lb_bois_recommend_info);

        btn_bois_recommend = new QPushButton(gb_bois_recommend);
        btn_bois_recommend->setObjectName("btn_bois_recommend");
        btn_bois_recommend->setMinimumSize(QSize(0, 45));

        vl_bois_recommend->addWidget(btn_bois_recommend);

        lb_bois_recommend_result = new QLabel(gb_bois_recommend);
        lb_bois_recommend_result->setObjectName("lb_bois_recommend_result");
        lb_bois_recommend_result->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:rgba(18,18,42,0.8);border:1px solid rgba(99,102,241,0.15);border-radius:8px;padding:12px;font-size:13px;"));
        lb_bois_recommend_result->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lb_bois_recommend_result->setWordWrap(true);

        vl_bois_recommend->addWidget(lb_bois_recommend_result);


        vl_bois_ai->addWidget(gb_bois_recommend);

        gb_bois_image_analyse = new QGroupBox(tab_bois_ai);
        gb_bois_image_analyse->setObjectName("gb_bois_image_analyse");
        vl_bois_image = new QVBoxLayout(gb_bois_image_analyse);
        vl_bois_image->setObjectName("vl_bois_image");
        lb_bois_image_info = new QLabel(gb_bois_image_analyse);
        lb_bois_image_info->setObjectName("lb_bois_image_info");
        lb_bois_image_info->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.75);background:transparent;font-size:13px;"));
        lb_bois_image_info->setWordWrap(true);

        vl_bois_image->addWidget(lb_bois_image_info);

        btn_bois_analyze_image = new QPushButton(gb_bois_image_analyse);
        btn_bois_analyze_image->setObjectName("btn_bois_analyze_image");
        btn_bois_analyze_image->setMinimumSize(QSize(0, 45));
        btn_bois_analyze_image->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #7c3aed,stop:1 #8b5cf6);"));

        vl_bois_image->addWidget(btn_bois_analyze_image);


        vl_bois_ai->addWidget(gb_bois_image_analyse);

        vs_bois_ai_spacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_bois_ai->addItem(vs_bois_ai_spacer);

        tabWidget_bois->addTab(tab_bois_ai, QString());
        tab_bois_form = new QWidget();
        tab_bois_form->setObjectName("tab_bois_form");
        vl_bois_form = new QVBoxLayout(tab_bois_form);
        vl_bois_form->setObjectName("vl_bois_form");
        vl_bois_form->setContentsMargins(20, 20, 20, 20);
        gf_bois = new QGroupBox(tab_bois_form);
        gf_bois->setObjectName("gf_bois");
        fl_bois = new QGridLayout(gf_bois);
        fl_bois->setObjectName("fl_bois");
        fl_bois->setHorizontalSpacing(15);
        fl_bois->setVerticalSpacing(8);
        lb1 = new QLabel(gf_bois);
        lb1->setObjectName("lb1");

        fl_bois->addWidget(lb1, 0, 0, 1, 1);

        bois_nom = new QLineEdit(gf_bois);
        bois_nom->setObjectName("bois_nom");
        bois_nom->setMaxLength(50);

        fl_bois->addWidget(bois_nom, 0, 1, 1, 1);

        lb2 = new QLabel(gf_bois);
        lb2->setObjectName("lb2");

        fl_bois->addWidget(lb2, 1, 0, 1, 1);

        bois_longueur = new QDoubleSpinBox(gf_bois);
        bois_longueur->setObjectName("bois_longueur");
        bois_longueur->setDecimals(2);
        bois_longueur->setMaximum(99999999.989999994635582);

        fl_bois->addWidget(bois_longueur, 9, 1, 1, 1);

        lb3 = new QLabel(gf_bois);
        lb3->setObjectName("lb3");

        fl_bois->addWidget(lb3, 2, 0, 1, 1);

        bois_largeur = new QDoubleSpinBox(gf_bois);
        bois_largeur->setObjectName("bois_largeur");
        bois_largeur->setDecimals(2);
        bois_largeur->setMaximum(99999999.989999994635582);

        fl_bois->addWidget(bois_largeur, 2, 1, 1, 1);

        lb4 = new QLabel(gf_bois);
        lb4->setObjectName("lb4");

        fl_bois->addWidget(lb4, 3, 0, 1, 1);

        bois_quantite = new QSpinBox(gf_bois);
        bois_quantite->setObjectName("bois_quantite");
        bois_quantite->setMaximum(999999);

        fl_bois->addWidget(bois_quantite, 3, 1, 1, 1);

        lb5 = new QLabel(gf_bois);
        lb5->setObjectName("lb5");

        fl_bois->addWidget(lb5, 4, 0, 1, 1);

        bois_prix = new QDoubleSpinBox(gf_bois);
        bois_prix->setObjectName("bois_prix");
        bois_prix->setDecimals(2);
        bois_prix->setMaximum(99999999.989999994635582);

        fl_bois->addWidget(bois_prix, 4, 1, 1, 1);

        lb6 = new QLabel(gf_bois);
        lb6->setObjectName("lb6");

        fl_bois->addWidget(lb6, 5, 0, 1, 1);

        bois_fournisseur = new QComboBox(gf_bois);
        bois_fournisseur->setObjectName("bois_fournisseur");

        fl_bois->addWidget(bois_fournisseur, 5, 1, 1, 1);


        vl_bois_form->addWidget(gf_bois);

        bl_bois = new QHBoxLayout();
        bl_bois->setObjectName("bl_bois");
        btn_bois_add = new QPushButton(tab_bois_form);
        btn_bois_add->setObjectName("btn_bois_add");

        bl_bois->addWidget(btn_bois_add);

        btn_bois_edit = new QPushButton(tab_bois_form);
        btn_bois_edit->setObjectName("btn_bois_edit");

        bl_bois->addWidget(btn_bois_edit);

        btn_bois_del = new QPushButton(tab_bois_form);
        btn_bois_del->setObjectName("btn_bois_del");
        btn_bois_del->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);"));

        bl_bois->addWidget(btn_bois_del);

        btn_bois_clr = new QPushButton(tab_bois_form);
        btn_bois_clr->setObjectName("btn_bois_clr");
        btn_bois_clr->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        bl_bois->addWidget(btn_bois_clr);


        vl_bois_form->addLayout(bl_bois);

        verticalSpacer_bois = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_bois_form->addItem(verticalSpacer_bois);

        tabWidget_bois->addTab(tab_bois_form, QString());
        stackedWidget->addWidget(Pbois);
        Pfabrication = new QWidget();
        Pfabrication->setObjectName("Pfabrication");
        c_Pfab = new QWidget(Pfabrication);
        c_Pfab->setObjectName("c_Pfab");
        c_Pfab->setGeometry(QRect(0, 0, 1201, 731));
        c_Pfab->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}QComboBox:hover{border:1px solid #6366f1;}QComboBox::drop-down{border:none;}QDateEdit,QSpinBox,QDoubleSpinBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-rad"
                        "ius:8px;padding:7px 10px;color:white;}QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}QTableView{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}QTableView::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}QScrollBar:vertical{border:none;background-color:transparent;width:8px;}QScrollBar::handle:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #818cf8,stop:1 #4f46e5);min-height:24px;border-"
                        "radius:4px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{border:none;background:none;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}"));
        t_fab = new QLabel(c_Pfab);
        t_fab->setObjectName("t_fab");
        t_fab->setGeometry(QRect(30, 15, 400, 35));
        t_fab->setFont(font);
        t_fab->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        tabWidget_fab = new QTabWidget(c_Pfab);
        tabWidget_fab->setObjectName("tabWidget_fab");
        tabWidget_fab->setGeometry(QRect(10, 55, 1181, 661));
        tab_fab_list = new QWidget();
        tab_fab_list->setObjectName("tab_fab_list");
        vl_fab_list = new QVBoxLayout(tab_fab_list);
        vl_fab_list->setObjectName("vl_fab_list");
        vl_fab_list->setContentsMargins(20, 20, 20, 20);
        hl_fab_top = new QHBoxLayout();
        hl_fab_top->setObjectName("hl_fab_top");
        search_fab = new QLineEdit(tab_fab_list);
        search_fab->setObjectName("search_fab");

        hl_fab_top->addWidget(search_fab);

        lb_filter_etat = new QLabel(tab_fab_list);
        lb_filter_etat->setObjectName("lb_filter_etat");
        lb_filter_etat->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_fab_top->addWidget(lb_filter_etat);

        combo_filter_etat = new QComboBox(tab_fab_list);
        combo_filter_etat->addItem(QString());
        combo_filter_etat->setObjectName("combo_filter_etat");

        hl_fab_top->addWidget(combo_filter_etat);


        vl_fab_list->addLayout(hl_fab_top);

        tv_fab = new QTableView(tab_fab_list);
        tv_fab->setObjectName("tv_fab");
        tv_fab->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_fab->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_fab->setShowGrid(false);
        tv_fab->setSortingEnabled(true);
        tv_fab->horizontalHeader()->setStretchLastSection(true);
        tv_fab->verticalHeader()->setVisible(false);
        tv_fab->verticalHeader()->setDefaultSectionSize(45);

        vl_fab_list->addWidget(tv_fab);

        hl_fab_actions = new QHBoxLayout();
        hl_fab_actions->setObjectName("hl_fab_actions");
        hs_fab_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_fab_actions->addItem(hs_fab_left);

        btn_fab_list_modify = new QPushButton(tab_fab_list);
        btn_fab_list_modify->setObjectName("btn_fab_list_modify");
        btn_fab_list_modify->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #2563eb,stop:1 #3b82f6);padding:10px 30px;"));

        hl_fab_actions->addWidget(btn_fab_list_modify);

        btn_fab_list_delete = new QPushButton(tab_fab_list);
        btn_fab_list_delete->setObjectName("btn_fab_list_delete");
        btn_fab_list_delete->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);padding:10px 30px;"));

        hl_fab_actions->addWidget(btn_fab_list_delete);

        btn_fab_export_pdf = new QPushButton(tab_fab_list);
        btn_fab_export_pdf->setObjectName("btn_fab_export_pdf");
        btn_fab_export_pdf->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:10px 30px;"));

        hl_fab_actions->addWidget(btn_fab_export_pdf);


        vl_fab_list->addLayout(hl_fab_actions);

        tabWidget_fab->addTab(tab_fab_list, QString());
        tab_fab_form = new QWidget();
        tab_fab_form->setObjectName("tab_fab_form");
        vl_fab_form = new QVBoxLayout(tab_fab_form);
        vl_fab_form->setObjectName("vl_fab_form");
        vl_fab_form->setContentsMargins(20, 20, 20, 20);
        gf_fab = new QGroupBox(tab_fab_form);
        gf_fab->setObjectName("gf_fab");
        fl_fab = new QGridLayout(gf_fab);
        fl_fab->setObjectName("fl_fab");
        fl_fab->setHorizontalSpacing(15);
        fl_fab->setVerticalSpacing(10);
        lf0 = new QLabel(gf_fab);
        lf0->setObjectName("lf0");

        fl_fab->addWidget(lf0, 0, 0, 1, 1);

        fab_id = new QLineEdit(gf_fab);
        fab_id->setObjectName("fab_id");
        fab_id->setMaxLength(10);

        fl_fab->addWidget(fab_id, 0, 1, 1, 1);

        lf1 = new QLabel(gf_fab);
        lf1->setObjectName("lf1");

        fl_fab->addWidget(lf1, 1, 0, 1, 1);

        fab_date_debut = new QDateEdit(gf_fab);
        fab_date_debut->setObjectName("fab_date_debut");
        fab_date_debut->setCalendarPopup(true);

        fl_fab->addWidget(fab_date_debut, 1, 1, 1, 1);

        lf2 = new QLabel(gf_fab);
        lf2->setObjectName("lf2");

        fl_fab->addWidget(lf2, 2, 0, 1, 1);

        fab_date_fin_prevue = new QDateEdit(gf_fab);
        fab_date_fin_prevue->setObjectName("fab_date_fin_prevue");
        fab_date_fin_prevue->setCalendarPopup(true);

        fl_fab->addWidget(fab_date_fin_prevue, 2, 1, 1, 1);

        lf3 = new QLabel(gf_fab);
        lf3->setObjectName("lf3");

        fl_fab->addWidget(lf3, 3, 0, 1, 1);

        fab_date_fin_reelle = new QDateEdit(gf_fab);
        fab_date_fin_reelle->setObjectName("fab_date_fin_reelle");
        fab_date_fin_reelle->setCalendarPopup(true);

        fl_fab->addWidget(fab_date_fin_reelle, 3, 1, 1, 1);

        lf4 = new QLabel(gf_fab);
        lf4->setObjectName("lf4");

        fl_fab->addWidget(lf4, 4, 0, 1, 1);

        fab_nb_personnes = new QSpinBox(gf_fab);
        fab_nb_personnes->setObjectName("fab_nb_personnes");
        fab_nb_personnes->setMaximum(999);

        fl_fab->addWidget(fab_nb_personnes, 4, 1, 1, 1);

        lf5 = new QLabel(gf_fab);
        lf5->setObjectName("lf5");

        fl_fab->addWidget(lf5, 5, 0, 1, 1);

        fab_etat = new QComboBox(gf_fab);
        fab_etat->setObjectName("fab_etat");

        fl_fab->addWidget(fab_etat, 5, 1, 1, 1);

        lf6 = new QLabel(gf_fab);
        lf6->setObjectName("lf6");

        fl_fab->addWidget(lf6, 6, 0, 1, 1);

        fab_idModele = new QComboBox(gf_fab);
        fab_idModele->setObjectName("fab_idModele");

        fl_fab->addWidget(fab_idModele, 6, 1, 1, 1);


        vl_fab_form->addWidget(gf_fab);

        bl_fab = new QHBoxLayout();
        bl_fab->setObjectName("bl_fab");
        btn_fab_add = new QPushButton(tab_fab_form);
        btn_fab_add->setObjectName("btn_fab_add");

        bl_fab->addWidget(btn_fab_add);

        btn_fab_edit = new QPushButton(tab_fab_form);
        btn_fab_edit->setObjectName("btn_fab_edit");

        bl_fab->addWidget(btn_fab_edit);

        btn_fab_del = new QPushButton(tab_fab_form);
        btn_fab_del->setObjectName("btn_fab_del");
        btn_fab_del->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);"));

        bl_fab->addWidget(btn_fab_del);

        btn_fab_clr = new QPushButton(tab_fab_form);
        btn_fab_clr->setObjectName("btn_fab_clr");
        btn_fab_clr->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        bl_fab->addWidget(btn_fab_clr);


        vl_fab_form->addLayout(bl_fab);

        vs_fab_btm = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_fab_form->addItem(vs_fab_btm);

        tabWidget_fab->addTab(tab_fab_form, QString());
        stackedWidget->addWidget(Pfabrication);
        Pmodele = new QWidget();
        Pmodele->setObjectName("Pmodele");
        c_Pmod = new QWidget(Pmodele);
        c_Pmod->setObjectName("c_Pmod");
        c_Pmod->setGeometry(QRect(0, 0, 1201, 731));
        c_Pmod->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}QComboBox:hover{border:1px solid #6366f1;}QComboBox::drop-down{border:none;}QDateEdit,QSpinBox,QDoubleSpinBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-rad"
                        "ius:8px;padding:7px 10px;color:white;}QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}QTableView{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}QTableView::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}QScrollBar:vertical{border:none;background-color:transparent;width:8px;}QScrollBar::handle:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #818cf8,stop:1 #4f46e5);min-height:24px;border-"
                        "radius:4px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{border:none;background:none;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}"));
        t_mod = new QLabel(c_Pmod);
        t_mod->setObjectName("t_mod");
        t_mod->setGeometry(QRect(30, 15, 400, 35));
        t_mod->setFont(font);
        t_mod->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        tabWidget_mod = new QTabWidget(c_Pmod);
        tabWidget_mod->setObjectName("tabWidget_mod");
        tabWidget_mod->setGeometry(QRect(10, 50, 1181, 671));
        tab_mod_list = new QWidget();
        tab_mod_list->setObjectName("tab_mod_list");
        vl_mod_list = new QVBoxLayout(tab_mod_list);
        vl_mod_list->setObjectName("vl_mod_list");
        vl_mod_list->setContentsMargins(20, 20, 20, 20);
        gl_mod = new QGroupBox(tab_mod_list);
        gl_mod->setObjectName("gl_mod");
        ll_mod = new QVBoxLayout(gl_mod);
        ll_mod->setObjectName("ll_mod");
        search_mod = new QLineEdit(gl_mod);
        search_mod->setObjectName("search_mod");

        ll_mod->addWidget(search_mod);

        tv_mod = new QTableView(gl_mod);
        tv_mod->setObjectName("tv_mod");
        tv_mod->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_mod->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_mod->setShowGrid(false);
        tv_mod->setSortingEnabled(true);
        tv_mod->horizontalHeader()->setStretchLastSection(true);
        tv_mod->verticalHeader()->setVisible(false);
        tv_mod->verticalHeader()->setDefaultSectionSize(45);

        ll_mod->addWidget(tv_mod);

        btn_mod_export_pdf = new QPushButton(gl_mod);
        btn_mod_export_pdf->setObjectName("btn_mod_export_pdf");
        btn_mod_export_pdf->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:8px 25px;"));

        ll_mod->addWidget(btn_mod_export_pdf);


        vl_mod_list->addWidget(gl_mod);

        tabWidget_mod->addTab(tab_mod_list, QString());
        tab_mod_form = new QWidget();
        tab_mod_form->setObjectName("tab_mod_form");
        vl_mod_form = new QVBoxLayout(tab_mod_form);
        vl_mod_form->setObjectName("vl_mod_form");
        vl_mod_form->setContentsMargins(20, 20, 20, 20);
        gf_mod = new QGroupBox(tab_mod_form);
        gf_mod->setObjectName("gf_mod");
        fl_mod = new QGridLayout(gf_mod);
        fl_mod->setObjectName("fl_mod");
        fl_mod->setHorizontalSpacing(15);
        fl_mod->setVerticalSpacing(8);
        lm0 = new QLabel(gf_mod);
        lm0->setObjectName("lm0");

        fl_mod->addWidget(lm0, 0, 0, 1, 1);

        mod_id = new QLineEdit(gf_mod);
        mod_id->setObjectName("mod_id");
        mod_id->setMaxLength(10);

        fl_mod->addWidget(mod_id, 0, 1, 1, 1);

        lm1 = new QLabel(gf_mod);
        lm1->setObjectName("lm1");

        fl_mod->addWidget(lm1, 1, 0, 1, 1);

        mod_nom = new QLineEdit(gf_mod);
        mod_nom->setObjectName("mod_nom");
        mod_nom->setMaxLength(100);

        fl_mod->addWidget(mod_nom, 1, 1, 1, 1);

        lm2 = new QLabel(gf_mod);
        lm2->setObjectName("lm2");

        fl_mod->addWidget(lm2, 2, 0, 1, 1);

        mod_categorie = new QComboBox(gf_mod);
        mod_categorie->setObjectName("mod_categorie");

        fl_mod->addWidget(mod_categorie, 2, 1, 1, 1);

        lm3 = new QLabel(gf_mod);
        lm3->setObjectName("lm3");

        fl_mod->addWidget(lm3, 3, 0, 1, 1);

        mod_dim = new QLineEdit(gf_mod);
        mod_dim->setObjectName("mod_dim");
        mod_dim->setMaxLength(100);

        fl_mod->addWidget(mod_dim, 3, 1, 1, 1);

        lm4 = new QLabel(gf_mod);
        lm4->setObjectName("lm4");

        fl_mod->addWidget(lm4, 4, 0, 1, 1);

        mod_type = new QComboBox(gf_mod);
        mod_type->setObjectName("mod_type");

        fl_mod->addWidget(mod_type, 4, 1, 1, 1);

        lm5 = new QLabel(gf_mod);
        lm5->setObjectName("lm5");

        fl_mod->addWidget(lm5, 5, 0, 1, 1);

        mod_prix = new QDoubleSpinBox(gf_mod);
        mod_prix->setObjectName("mod_prix");
        mod_prix->setDecimals(2);
        mod_prix->setMaximum(99999999.989999994635582);

        fl_mod->addWidget(mod_prix, 5, 1, 1, 1);

        lm6 = new QLabel(gf_mod);
        lm6->setObjectName("lm6");

        fl_mod->addWidget(lm6, 6, 0, 1, 1);

        mod_desc = new QTextEdit(gf_mod);
        mod_desc->setObjectName("mod_desc");
        mod_desc->setMaximumSize(QSize(16777215, 60));

        fl_mod->addWidget(mod_desc, 6, 1, 1, 1);

        lm7 = new QLabel(gf_mod);
        lm7->setObjectName("lm7");

        fl_mod->addWidget(lm7, 7, 0, 1, 1);

        mod_cout = new QDoubleSpinBox(gf_mod);
        mod_cout->setObjectName("mod_cout");
        mod_cout->setDecimals(2);
        mod_cout->setMaximum(99999999.989999994635582);

        fl_mod->addWidget(mod_cout, 7, 1, 1, 1);


        vl_mod_form->addWidget(gf_mod);

        bb_mod = new QWidget(tab_mod_form);
        bb_mod->setObjectName("bb_mod");
        bb_mod->setStyleSheet(QString::fromUtf8("background:transparent;"));
        bl_mod = new QHBoxLayout(bb_mod);
        bl_mod->setObjectName("bl_mod");
        btn_mod_add = new QPushButton(bb_mod);
        btn_mod_add->setObjectName("btn_mod_add");

        bl_mod->addWidget(btn_mod_add);

        btn_mod_edit = new QPushButton(bb_mod);
        btn_mod_edit->setObjectName("btn_mod_edit");

        bl_mod->addWidget(btn_mod_edit);

        btn_mod_del = new QPushButton(bb_mod);
        btn_mod_del->setObjectName("btn_mod_del");
        btn_mod_del->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);"));

        bl_mod->addWidget(btn_mod_del);

        btn_mod_clr = new QPushButton(bb_mod);
        btn_mod_clr->setObjectName("btn_mod_clr");
        btn_mod_clr->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        bl_mod->addWidget(btn_mod_clr);


        vl_mod_form->addWidget(bb_mod);

        vs_mod_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_mod_form->addItem(vs_mod_bottom);

        tabWidget_mod->addTab(tab_mod_form, QString());
        tab_mod_stats = new QWidget();
        tab_mod_stats->setObjectName("tab_mod_stats");
        vl_mod_stats = new QVBoxLayout(tab_mod_stats);
        vl_mod_stats->setObjectName("vl_mod_stats");
        vl_mod_stats->setContentsMargins(20, 20, 20, 20);
        hl_mod_stats_top = new QHBoxLayout();
        hl_mod_stats_top->setObjectName("hl_mod_stats_top");
        gb_mod_stats_total = new QGroupBox(tab_mod_stats);
        gb_mod_stats_total->setObjectName("gb_mod_stats_total");
        gl_mod_stats_total = new QGridLayout(gb_mod_stats_total);
        gl_mod_stats_total->setObjectName("gl_mod_stats_total");
        lb_mod_stats_total_title = new QLabel(gb_mod_stats_total);
        lb_mod_stats_total_title->setObjectName("lb_mod_stats_total_title");

        gl_mod_stats_total->addWidget(lb_mod_stats_total_title, 0, 0, 1, 1);

        lb_mod_stats_total = new QLabel(gb_mod_stats_total);
        lb_mod_stats_total->setObjectName("lb_mod_stats_total");

        gl_mod_stats_total->addWidget(lb_mod_stats_total, 0, 1, 1, 1);

        lb_mod_stats_avg_price_title = new QLabel(gb_mod_stats_total);
        lb_mod_stats_avg_price_title->setObjectName("lb_mod_stats_avg_price_title");

        gl_mod_stats_total->addWidget(lb_mod_stats_avg_price_title, 1, 0, 1, 1);

        lb_mod_stats_avg_price = new QLabel(gb_mod_stats_total);
        lb_mod_stats_avg_price->setObjectName("lb_mod_stats_avg_price");

        gl_mod_stats_total->addWidget(lb_mod_stats_avg_price, 1, 1, 1, 1);


        hl_mod_stats_top->addWidget(gb_mod_stats_total);

        gb_mod_stats_price = new QGroupBox(tab_mod_stats);
        gb_mod_stats_price->setObjectName("gb_mod_stats_price");
        gl_mod_stats_price = new QGridLayout(gb_mod_stats_price);
        gl_mod_stats_price->setObjectName("gl_mod_stats_price");
        lb_mod_stats_min_price_title = new QLabel(gb_mod_stats_price);
        lb_mod_stats_min_price_title->setObjectName("lb_mod_stats_min_price_title");

        gl_mod_stats_price->addWidget(lb_mod_stats_min_price_title, 0, 0, 1, 1);

        lb_mod_stats_min_price = new QLabel(gb_mod_stats_price);
        lb_mod_stats_min_price->setObjectName("lb_mod_stats_min_price");

        gl_mod_stats_price->addWidget(lb_mod_stats_min_price, 0, 1, 1, 1);

        lb_mod_stats_max_price_title = new QLabel(gb_mod_stats_price);
        lb_mod_stats_max_price_title->setObjectName("lb_mod_stats_max_price_title");

        gl_mod_stats_price->addWidget(lb_mod_stats_max_price_title, 1, 0, 1, 1);

        lb_mod_stats_max_price = new QLabel(gb_mod_stats_price);
        lb_mod_stats_max_price->setObjectName("lb_mod_stats_max_price");

        gl_mod_stats_price->addWidget(lb_mod_stats_max_price, 1, 1, 1, 1);


        hl_mod_stats_top->addWidget(gb_mod_stats_price);

        gb_mod_stats_cost = new QGroupBox(tab_mod_stats);
        gb_mod_stats_cost->setObjectName("gb_mod_stats_cost");
        gl_mod_stats_cost = new QGridLayout(gb_mod_stats_cost);
        gl_mod_stats_cost->setObjectName("gl_mod_stats_cost");
        lb_mod_stats_avg_cost_title = new QLabel(gb_mod_stats_cost);
        lb_mod_stats_avg_cost_title->setObjectName("lb_mod_stats_avg_cost_title");

        gl_mod_stats_cost->addWidget(lb_mod_stats_avg_cost_title, 0, 0, 1, 1);

        lb_mod_stats_avg_cost = new QLabel(gb_mod_stats_cost);
        lb_mod_stats_avg_cost->setObjectName("lb_mod_stats_avg_cost");

        gl_mod_stats_cost->addWidget(lb_mod_stats_avg_cost, 0, 1, 1, 1);


        hl_mod_stats_top->addWidget(gb_mod_stats_cost);


        vl_mod_stats->addLayout(hl_mod_stats_top);

        hl_mod_stats_tables = new QHBoxLayout();
        hl_mod_stats_tables->setObjectName("hl_mod_stats_tables");
        gb_mod_stats_top = new QGroupBox(tab_mod_stats);
        gb_mod_stats_top->setObjectName("gb_mod_stats_top");
        vl_mod_stats_top = new QVBoxLayout(gb_mod_stats_top);
        vl_mod_stats_top->setObjectName("vl_mod_stats_top");
        tv_mod_stats_top = new QTableView(gb_mod_stats_top);
        tv_mod_stats_top->setObjectName("tv_mod_stats_top");
        tv_mod_stats_top->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_mod_stats_top->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_mod_stats_top->setShowGrid(false);
        tv_mod_stats_top->setSortingEnabled(true);
        tv_mod_stats_top->horizontalHeader()->setStretchLastSection(true);
        tv_mod_stats_top->verticalHeader()->setVisible(false);
        tv_mod_stats_top->verticalHeader()->setDefaultSectionSize(42);

        vl_mod_stats_top->addWidget(tv_mod_stats_top);


        hl_mod_stats_tables->addWidget(gb_mod_stats_top);

        gb_mod_stats_cat = new QGroupBox(tab_mod_stats);
        gb_mod_stats_cat->setObjectName("gb_mod_stats_cat");
        vl_mod_stats_cat = new QVBoxLayout(gb_mod_stats_cat);
        vl_mod_stats_cat->setObjectName("vl_mod_stats_cat");
        tv_mod_stats_cat = new QTableView(gb_mod_stats_cat);
        tv_mod_stats_cat->setObjectName("tv_mod_stats_cat");
        tv_mod_stats_cat->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_mod_stats_cat->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_mod_stats_cat->setShowGrid(false);
        tv_mod_stats_cat->setSortingEnabled(true);
        tv_mod_stats_cat->horizontalHeader()->setStretchLastSection(true);
        tv_mod_stats_cat->verticalHeader()->setVisible(false);
        tv_mod_stats_cat->verticalHeader()->setDefaultSectionSize(42);

        vl_mod_stats_cat->addWidget(tv_mod_stats_cat);


        hl_mod_stats_tables->addWidget(gb_mod_stats_cat);


        vl_mod_stats->addLayout(hl_mod_stats_tables);

        hl_mod_stats_charts = new QHBoxLayout();
        hl_mod_stats_charts->setObjectName("hl_mod_stats_charts");
        gb_mod_stats_chart_cat = new QGroupBox(tab_mod_stats);
        gb_mod_stats_chart_cat->setObjectName("gb_mod_stats_chart_cat");
        vl_mod_stats_chart_cat = new QVBoxLayout(gb_mod_stats_chart_cat);
        vl_mod_stats_chart_cat->setObjectName("vl_mod_stats_chart_cat");
        mod_stats_chart_categories = new QChartView(gb_mod_stats_chart_cat);
        mod_stats_chart_categories->setObjectName("mod_stats_chart_categories");

        vl_mod_stats_chart_cat->addWidget(mod_stats_chart_categories);


        hl_mod_stats_charts->addWidget(gb_mod_stats_chart_cat);

        gb_mod_stats_chart_price = new QGroupBox(tab_mod_stats);
        gb_mod_stats_chart_price->setObjectName("gb_mod_stats_chart_price");
        vl_mod_stats_chart_price = new QVBoxLayout(gb_mod_stats_chart_price);
        vl_mod_stats_chart_price->setObjectName("vl_mod_stats_chart_price");
        mod_stats_chart_price = new QChartView(gb_mod_stats_chart_price);
        mod_stats_chart_price->setObjectName("mod_stats_chart_price");

        vl_mod_stats_chart_price->addWidget(mod_stats_chart_price);


        hl_mod_stats_charts->addWidget(gb_mod_stats_chart_price);


        vl_mod_stats->addLayout(hl_mod_stats_charts);

        btn_mod_stats_refresh = new QPushButton(tab_mod_stats);
        btn_mod_stats_refresh->setObjectName("btn_mod_stats_refresh");

        vl_mod_stats->addWidget(btn_mod_stats_refresh);

        vs_mod_stats_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_mod_stats->addItem(vs_mod_stats_bottom);

        tabWidget_mod->addTab(tab_mod_stats, QString());
        tab_mod_estimation = new QWidget();
        tab_mod_estimation->setObjectName("tab_mod_estimation");
        vl_mod_estimation = new QVBoxLayout(tab_mod_estimation);
        vl_mod_estimation->setObjectName("vl_mod_estimation");
        vl_mod_estimation->setContentsMargins(20, 20, 20, 20);
        gb_mod_estimation = new QGroupBox(tab_mod_estimation);
        gb_mod_estimation->setObjectName("gb_mod_estimation");
        vl_mod_estimation_box = new QVBoxLayout(gb_mod_estimation);
        vl_mod_estimation_box->setObjectName("vl_mod_estimation_box");
        lb_mod_estimate_hint = new QLabel(gb_mod_estimation);
        lb_mod_estimate_hint->setObjectName("lb_mod_estimate_hint");

        vl_mod_estimation_box->addWidget(lb_mod_estimate_hint);

        btn_mod_estimate_image = new QPushButton(gb_mod_estimation);
        btn_mod_estimate_image->setObjectName("btn_mod_estimate_image");

        vl_mod_estimation_box->addWidget(btn_mod_estimate_image);


        vl_mod_estimation->addWidget(gb_mod_estimation);

        vs_mod_estimation_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_mod_estimation->addItem(vs_mod_estimation_bottom);

        tabWidget_mod->addTab(tab_mod_estimation, QString());
        tab_mod_budget = new QWidget();
        tab_mod_budget->setObjectName("tab_mod_budget");
        vl_mod_budget = new QVBoxLayout(tab_mod_budget);
        vl_mod_budget->setObjectName("vl_mod_budget");
        vl_mod_budget->setContentsMargins(20, 20, 20, 20);
        gb_mod_budget = new QGroupBox(tab_mod_budget);
        gb_mod_budget->setObjectName("gb_mod_budget");
        gl_mod_budget = new QGridLayout(gb_mod_budget);
        gl_mod_budget->setObjectName("gl_mod_budget");
        lb_mod_budget_label = new QLabel(gb_mod_budget);
        lb_mod_budget_label->setObjectName("lb_mod_budget_label");

        gl_mod_budget->addWidget(lb_mod_budget_label, 0, 0, 1, 1);

        ds_mod_budget = new QDoubleSpinBox(gb_mod_budget);
        ds_mod_budget->setObjectName("ds_mod_budget");
        ds_mod_budget->setDecimals(2);
        ds_mod_budget->setMaximum(99999999.989999994635582);

        gl_mod_budget->addWidget(ds_mod_budget, 0, 1, 1, 1);

        btn_mod_budget_generate = new QPushButton(gb_mod_budget);
        btn_mod_budget_generate->setObjectName("btn_mod_budget_generate");

        gl_mod_budget->addWidget(btn_mod_budget_generate, 1, 0, 1, 2);

        lb_mod_budget_result = new QLabel(gb_mod_budget);
        lb_mod_budget_result->setObjectName("lb_mod_budget_result");

        gl_mod_budget->addWidget(lb_mod_budget_result, 2, 0, 1, 2);


        vl_mod_budget->addWidget(gb_mod_budget);

        vs_mod_budget_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_mod_budget->addItem(vs_mod_budget_bottom);

        tabWidget_mod->addTab(tab_mod_budget, QString());
        tab_mod_performance = new QWidget();
        tab_mod_performance->setObjectName("tab_mod_performance");
        vl_mod_perf_root = new QVBoxLayout(tab_mod_performance);
        vl_mod_perf_root->setSpacing(14);
        vl_mod_perf_root->setObjectName("vl_mod_perf_root");
        vl_mod_perf_root->setContentsMargins(16, 16, 16, 16);
        hl_mod_perf = new QHBoxLayout();
        hl_mod_perf->setSpacing(16);
        hl_mod_perf->setObjectName("hl_mod_perf");
        gb_mod_perf_entry = new QGroupBox(tab_mod_performance);
        gb_mod_perf_entry->setObjectName("gb_mod_perf_entry");
        gb_mod_perf_entry->setMinimumSize(QSize(320, 0));
        gl_mod_perf_entry = new QGridLayout(gb_mod_perf_entry);
        gl_mod_perf_entry->setObjectName("gl_mod_perf_entry");
        gl_mod_perf_entry->setHorizontalSpacing(10);
        gl_mod_perf_entry->setVerticalSpacing(10);
        lb_mod_perf_type = new QLabel(gb_mod_perf_entry);
        lb_mod_perf_type->setObjectName("lb_mod_perf_type");

        gl_mod_perf_entry->addWidget(lb_mod_perf_type, 0, 0, 1, 1);

        cb_mod_perf_type = new QComboBox(gb_mod_perf_entry);
        cb_mod_perf_type->setObjectName("cb_mod_perf_type");

        gl_mod_perf_entry->addWidget(cb_mod_perf_type, 0, 1, 1, 1);

        lb_mod_perf_dimensions = new QLabel(gb_mod_perf_entry);
        lb_mod_perf_dimensions->setObjectName("lb_mod_perf_dimensions");

        gl_mod_perf_entry->addWidget(lb_mod_perf_dimensions, 1, 0, 1, 1);

        le_mod_perf_dimensions = new QLineEdit(gb_mod_perf_entry);
        le_mod_perf_dimensions->setObjectName("le_mod_perf_dimensions");

        gl_mod_perf_entry->addWidget(le_mod_perf_dimensions, 1, 1, 1, 1);

        lb_mod_perf_materiau = new QLabel(gb_mod_perf_entry);
        lb_mod_perf_materiau->setObjectName("lb_mod_perf_materiau");

        gl_mod_perf_entry->addWidget(lb_mod_perf_materiau, 2, 0, 1, 1);

        cb_mod_perf_materiau = new QComboBox(gb_mod_perf_entry);
        cb_mod_perf_materiau->setObjectName("cb_mod_perf_materiau");

        gl_mod_perf_entry->addWidget(cb_mod_perf_materiau, 2, 1, 1, 1);

        lb_mod_perf_complexite = new QLabel(gb_mod_perf_entry);
        lb_mod_perf_complexite->setObjectName("lb_mod_perf_complexite");

        gl_mod_perf_entry->addWidget(lb_mod_perf_complexite, 3, 0, 1, 1);

        cb_mod_perf_complexite = new QComboBox(gb_mod_perf_entry);
        cb_mod_perf_complexite->setObjectName("cb_mod_perf_complexite");

        gl_mod_perf_entry->addWidget(cb_mod_perf_complexite, 3, 1, 1, 1);

        lb_mod_perf_quantite = new QLabel(gb_mod_perf_entry);
        lb_mod_perf_quantite->setObjectName("lb_mod_perf_quantite");

        gl_mod_perf_entry->addWidget(lb_mod_perf_quantite, 4, 0, 1, 1);

        sb_mod_perf_quantite = new QSpinBox(gb_mod_perf_entry);
        sb_mod_perf_quantite->setObjectName("sb_mod_perf_quantite");
        sb_mod_perf_quantite->setMinimum(1);
        sb_mod_perf_quantite->setMaximum(500);
        sb_mod_perf_quantite->setValue(1);

        gl_mod_perf_entry->addWidget(sb_mod_perf_quantite, 4, 1, 1, 1);

        btn_mod_perf_calculate = new QPushButton(gb_mod_perf_entry);
        btn_mod_perf_calculate->setObjectName("btn_mod_perf_calculate");

        gl_mod_perf_entry->addWidget(btn_mod_perf_calculate, 5, 0, 1, 2);


        hl_mod_perf->addWidget(gb_mod_perf_entry);

        gb_mod_perf_results = new QGroupBox(tab_mod_performance);
        gb_mod_perf_results->setObjectName("gb_mod_perf_results");
        vl_mod_perf_results = new QVBoxLayout(gb_mod_perf_results);
        vl_mod_perf_results->setSpacing(8);
        vl_mod_perf_results->setObjectName("vl_mod_perf_results");
        vl_mod_perf_results->setContentsMargins(10, 10, 10, 10);
        tb_mod_perf_result = new QTextBrowser(gb_mod_perf_results);
        tb_mod_perf_result->setObjectName("tb_mod_perf_result");
        tb_mod_perf_result->setReadOnly(true);
        tb_mod_perf_result->setOpenExternalLinks(false);

        vl_mod_perf_results->addWidget(tb_mod_perf_result);


        hl_mod_perf->addWidget(gb_mod_perf_results);


        vl_mod_perf_root->addLayout(hl_mod_perf);

        gb_mod_perf_chart = new QGroupBox(tab_mod_performance);
        gb_mod_perf_chart->setObjectName("gb_mod_perf_chart");
        gb_mod_perf_chart->setMinimumSize(QSize(0, 240));
        vl_mod_perf_chart = new QVBoxLayout(gb_mod_perf_chart);
        vl_mod_perf_chart->setSpacing(0);
        vl_mod_perf_chart->setObjectName("vl_mod_perf_chart");
        vl_mod_perf_chart->setContentsMargins(6, 6, 6, 6);
        w_mod_perf_chart_placeholder = new QWidget(gb_mod_perf_chart);
        w_mod_perf_chart_placeholder->setObjectName("w_mod_perf_chart_placeholder");

        vl_mod_perf_chart->addWidget(w_mod_perf_chart_placeholder);


        vl_mod_perf_root->addWidget(gb_mod_perf_chart);

        hl_mod_perf_footer = new QHBoxLayout();
        hl_mod_perf_footer->setObjectName("hl_mod_perf_footer");
        btn_mod_perf_export_pdf = new QPushButton(tab_mod_performance);
        btn_mod_perf_export_pdf->setObjectName("btn_mod_perf_export_pdf");

        hl_mod_perf_footer->addWidget(btn_mod_perf_export_pdf);

        hs_mod_perf_footer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_mod_perf_footer->addItem(hs_mod_perf_footer);


        vl_mod_perf_root->addLayout(hl_mod_perf_footer);

        tabWidget_mod->addTab(tab_mod_performance, QString());
        stackedWidget->addWidget(Pmodele);
        Ppieces = new QWidget();
        Ppieces->setObjectName("Ppieces");
        c_Ppc = new QWidget(Ppieces);
        c_Ppc->setObjectName("c_Ppc");
        c_Ppc->setGeometry(QRect(-10, -10, 1201, 731));
        c_Ppc->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit,QTextEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus,QTextEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QComboBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:7px 10px;color:white;}QComboBox:hover{border:1px solid #6366f1;}QComboBox::drop-down{border:none;}QDateEdit,QSpinBox,QDoubleSpinBox{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-rad"
                        "ius:8px;padding:7px 10px;color:white;}QPushButton{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:1 #4f46e5);}QTableView{border:none;font-size:14px;background-color:#13132b;gridline-color:rgba(99,102,241,0.08);}QTableView::item:selected{background-color:rgba(99,102,241,0.25);color:#FFFFFF;}QHeaderView{font-size:14px;background-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #3730a3,stop:0.5 #4f46e5,stop:1 #6366f1);color:#FFFFFF;border:none;}QHeaderView::section:horizontal{padding:6px;border:none;background-color:transparent;}QScrollBar:vertical{border:none;background-color:transparent;width:8px;}QScrollBar::handle:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #818cf8,stop:1 #4f46e5);min-height:24px;border-"
                        "radius:4px;}QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{border:none;background:none;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}QTabWidget::pane{border:1px solid rgba(99,102,241,0.2);border-radius:10px;background-color:rgba(18,18,42,0.9);top:-1px;}QTabBar::tab{background:rgba(30,30,60,0.6);color:#ffffff;border:1px solid rgba(99,102,241,0.15);border-bottom:none;border-top-left-radius:12px;border-top-right-radius:12px;padding:6px 15px;margin-right:4px;font-weight:600;}QTabBar::tab:selected{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;}QTabBar::tab:hover:!selected{background:rgba(99,102,241,0.15);color:white;}"));
        t_pc = new QLabel(c_Ppc);
        t_pc->setObjectName("t_pc");
        t_pc->setGeometry(QRect(30, 10, 400, 35));
        t_pc->setFont(font);
        t_pc->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        tabWidget_pc = new QTabWidget(c_Ppc);
        tabWidget_pc->setObjectName("tabWidget_pc");
        tabWidget_pc->setGeometry(QRect(10, 50, 1181, 671));
        tab_pc_list = new QWidget();
        tab_pc_list->setObjectName("tab_pc_list");
        vl_pc_list = new QVBoxLayout(tab_pc_list);
        vl_pc_list->setObjectName("vl_pc_list");
        vl_pc_list->setContentsMargins(20, 20, 20, 20);
        hl_pc_top = new QHBoxLayout();
        hl_pc_top->setObjectName("hl_pc_top");
        search_pc = new QLineEdit(tab_pc_list);
        search_pc->setObjectName("search_pc");

        hl_pc_top->addWidget(search_pc);

        lb_filter_matiere = new QLabel(tab_pc_list);
        lb_filter_matiere->setObjectName("lb_filter_matiere");
        lb_filter_matiere->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_pc_top->addWidget(lb_filter_matiere);

        combo_filter_matiere = new QComboBox(tab_pc_list);
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->addItem(QString());
        combo_filter_matiere->setObjectName("combo_filter_matiere");

        hl_pc_top->addWidget(combo_filter_matiere);

        lb_pc_sort_by = new QLabel(tab_pc_list);
        lb_pc_sort_by->setObjectName("lb_pc_sort_by");
        lb_pc_sort_by->setStyleSheet(QString::fromUtf8("background:transparent;"));

        hl_pc_top->addWidget(lb_pc_sort_by);

        pc_sort_by = new QComboBox(tab_pc_list);
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->addItem(QString());
        pc_sort_by->setObjectName("pc_sort_by");

        hl_pc_top->addWidget(pc_sort_by);

        pc_sort_order = new QComboBox(tab_pc_list);
        pc_sort_order->addItem(QString());
        pc_sort_order->addItem(QString());
        pc_sort_order->setObjectName("pc_sort_order");

        hl_pc_top->addWidget(pc_sort_order);


        vl_pc_list->addLayout(hl_pc_top);

        tv_pc = new QTableView(tab_pc_list);
        tv_pc->setObjectName("tv_pc");
        tv_pc->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tv_pc->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tv_pc->setShowGrid(false);
        tv_pc->setSortingEnabled(true);
        tv_pc->horizontalHeader()->setStretchLastSection(true);
        tv_pc->verticalHeader()->setVisible(false);
        tv_pc->verticalHeader()->setDefaultSectionSize(45);

        vl_pc_list->addWidget(tv_pc);

        hl_pc_actions = new QHBoxLayout();
        hl_pc_actions->setObjectName("hl_pc_actions");
        hs_pc_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hl_pc_actions->addItem(hs_pc_left);

        btn_pc_list_modify = new QPushButton(tab_pc_list);
        btn_pc_list_modify->setObjectName("btn_pc_list_modify");
        btn_pc_list_modify->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #2563eb,stop:1 #3b82f6);padding:8px 25px;"));

        hl_pc_actions->addWidget(btn_pc_list_modify);

        btn_pc_list_delete = new QPushButton(tab_pc_list);
        btn_pc_list_delete->setObjectName("btn_pc_list_delete");
        btn_pc_list_delete->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);padding:8px 25px;"));

        hl_pc_actions->addWidget(btn_pc_list_delete);

        btn_pc_export_pdf = new QPushButton(tab_pc_list);
        btn_pc_export_pdf->setObjectName("btn_pc_export_pdf");
        btn_pc_export_pdf->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #b45309,stop:1 #f59e0b);padding:8px 25px;"));

        hl_pc_actions->addWidget(btn_pc_export_pdf);


        vl_pc_list->addLayout(hl_pc_actions);

        tabWidget_pc->addTab(tab_pc_list, QString());
        tab_pc_form = new QWidget();
        tab_pc_form->setObjectName("tab_pc_form");
        vl_pc_form = new QVBoxLayout(tab_pc_form);
        vl_pc_form->setObjectName("vl_pc_form");
        vl_pc_form->setContentsMargins(20, 20, 20, 20);
        gf_pc = new QGroupBox(tab_pc_form);
        gf_pc->setObjectName("gf_pc");
        fl_pc = new QGridLayout(gf_pc);
        fl_pc->setObjectName("fl_pc");
        fl_pc->setHorizontalSpacing(15);
        fl_pc->setVerticalSpacing(8);
        lp0 = new QLabel(gf_pc);
        lp0->setObjectName("lp0");

        fl_pc->addWidget(lp0, 0, 0, 1, 1);

        pc_id = new QLineEdit(gf_pc);
        pc_id->setObjectName("pc_id");
        pc_id->setMaxLength(10);
        pc_id->setReadOnly(true);

        fl_pc->addWidget(pc_id, 0, 1, 1, 1);

        lp1 = new QLabel(gf_pc);
        lp1->setObjectName("lp1");

        fl_pc->addWidget(lp1, 1, 0, 1, 1);

        pc_nom = new QLineEdit(gf_pc);
        pc_nom->setObjectName("pc_nom");
        pc_nom->setMaxLength(100);

        fl_pc->addWidget(pc_nom, 1, 1, 1, 1);

        lp2 = new QLabel(gf_pc);
        lp2->setObjectName("lp2");

        fl_pc->addWidget(lp2, 2, 0, 1, 1);

        pc_code = new QLineEdit(gf_pc);
        pc_code->setObjectName("pc_code");
        pc_code->setMaxLength(50);

        fl_pc->addWidget(pc_code, 2, 1, 1, 1);

        lp3 = new QLabel(gf_pc);
        lp3->setObjectName("lp3");

        fl_pc->addWidget(lp3, 3, 0, 1, 1);

        pc_qty = new QSpinBox(gf_pc);
        pc_qty->setObjectName("pc_qty");
        pc_qty->setMaximum(999999);

        fl_pc->addWidget(pc_qty, 3, 1, 1, 1);

        lp4 = new QLabel(gf_pc);
        lp4->setObjectName("lp4");

        fl_pc->addWidget(lp4, 4, 0, 1, 1);

        pc_matiere = new QComboBox(gf_pc);
        pc_matiere->setObjectName("pc_matiere");

        fl_pc->addWidget(pc_matiere, 4, 1, 1, 1);

        lp5 = new QLabel(gf_pc);
        lp5->setObjectName("lp5");

        fl_pc->addWidget(lp5, 5, 0, 1, 1);

        pc_long = new QDoubleSpinBox(gf_pc);
        pc_long->setObjectName("pc_long");
        pc_long->setDecimals(2);
        pc_long->setMaximum(99999.990000000005239);

        fl_pc->addWidget(pc_long, 5, 1, 1, 1);

        lp6 = new QLabel(gf_pc);
        lp6->setObjectName("lp6");

        fl_pc->addWidget(lp6, 6, 0, 1, 1);

        pc_larg = new QDoubleSpinBox(gf_pc);
        pc_larg->setObjectName("pc_larg");
        pc_larg->setDecimals(2);
        pc_larg->setMaximum(99999.990000000005239);

        fl_pc->addWidget(pc_larg, 6, 1, 1, 1);

        lp7 = new QLabel(gf_pc);
        lp7->setObjectName("lp7");

        fl_pc->addWidget(lp7, 7, 0, 1, 1);

        pc_epais = new QDoubleSpinBox(gf_pc);
        pc_epais->setObjectName("pc_epais");
        pc_epais->setDecimals(2);
        pc_epais->setMaximum(99999.990000000005239);

        fl_pc->addWidget(pc_epais, 7, 1, 1, 1);

        lp8 = new QLabel(gf_pc);
        lp8->setObjectName("lp8");

        fl_pc->addWidget(lp8, 8, 0, 1, 1);

        pc_modele = new QComboBox(gf_pc);
        pc_modele->setObjectName("pc_modele");

        fl_pc->addWidget(pc_modele, 8, 1, 1, 1);

        lp9 = new QLabel(gf_pc);
        lp9->setObjectName("lp9");

        fl_pc->addWidget(lp9, 9, 0, 1, 1);

        pc_idModele = new QSpinBox(gf_pc);
        pc_idModele->setObjectName("pc_idModele");
        pc_idModele->setReadOnly(true);
        pc_idModele->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        pc_idModele->setMinimum(0);
        pc_idModele->setMaximum(999999);

        fl_pc->addWidget(pc_idModele, 9, 1, 1, 1);

        lp10 = new QLabel(gf_pc);
        lp10->setObjectName("lp10");

        fl_pc->addWidget(lp10, 10, 0, 1, 1);

        pc_idBois = new QSpinBox(gf_pc);
        pc_idBois->setObjectName("pc_idBois");
        pc_idBois->setReadOnly(true);
        pc_idBois->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        pc_idBois->setMinimum(0);
        pc_idBois->setMaximum(999999);

        fl_pc->addWidget(pc_idBois, 10, 1, 1, 1);


        vl_pc_form->addWidget(gf_pc);

        bl_pc = new QHBoxLayout();
        bl_pc->setObjectName("bl_pc");
        btn_pc_add = new QPushButton(tab_pc_form);
        btn_pc_add->setObjectName("btn_pc_add");

        bl_pc->addWidget(btn_pc_add);

        btn_pc_edit = new QPushButton(tab_pc_form);
        btn_pc_edit->setObjectName("btn_pc_edit");

        bl_pc->addWidget(btn_pc_edit);

        btn_pc_del = new QPushButton(tab_pc_form);
        btn_pc_del->setObjectName("btn_pc_del");
        btn_pc_del->setStyleSheet(QString::fromUtf8("background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #dc2626,stop:1 #ef4444);"));

        bl_pc->addWidget(btn_pc_del);

        btn_pc_clr = new QPushButton(tab_pc_form);
        btn_pc_clr->setObjectName("btn_pc_clr");
        btn_pc_clr->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        bl_pc->addWidget(btn_pc_clr);


        vl_pc_form->addLayout(bl_pc);

        verticalSpacer_pc = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_pc_form->addItem(verticalSpacer_pc);

        tabWidget_pc->addTab(tab_pc_form, QString());
        tab_pc_stats = new QWidget();
        tab_pc_stats->setObjectName("tab_pc_stats");
        vl_pc_stats = new QVBoxLayout(tab_pc_stats);
        vl_pc_stats->setSpacing(18);
        vl_pc_stats->setObjectName("vl_pc_stats");
        vl_pc_stats->setContentsMargins(20, 20, 20, 20);
        hl_pc_stats_cards = new QHBoxLayout();
        hl_pc_stats_cards->setSpacing(16);
        hl_pc_stats_cards->setObjectName("hl_pc_stats_cards");
        gb_pc_stats_total = new QGroupBox(tab_pc_stats);
        gb_pc_stats_total->setObjectName("gb_pc_stats_total");
        vl_pc_stats_total = new QVBoxLayout(gb_pc_stats_total);
        vl_pc_stats_total->setObjectName("vl_pc_stats_total");
        lb_pc_stats_total_title = new QLabel(gb_pc_stats_total);
        lb_pc_stats_total_title->setObjectName("lb_pc_stats_total_title");
        lb_pc_stats_total_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_pc_stats_total->addWidget(lb_pc_stats_total_title);

        lb_pc_stats_total_value = new QLabel(gb_pc_stats_total);
        lb_pc_stats_total_value->setObjectName("lb_pc_stats_total_value");
        lb_pc_stats_total_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:28px;font-weight:700;"));

        vl_pc_stats_total->addWidget(lb_pc_stats_total_value);

        lb_pc_stats_total_hint = new QLabel(gb_pc_stats_total);
        lb_pc_stats_total_hint->setObjectName("lb_pc_stats_total_hint");
        lb_pc_stats_total_hint->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.65);background:transparent;"));

        vl_pc_stats_total->addWidget(lb_pc_stats_total_hint);


        hl_pc_stats_cards->addWidget(gb_pc_stats_total);

        gb_pc_stats_yield = new QGroupBox(tab_pc_stats);
        gb_pc_stats_yield->setObjectName("gb_pc_stats_yield");
        vl_pc_stats_yield = new QVBoxLayout(gb_pc_stats_yield);
        vl_pc_stats_yield->setObjectName("vl_pc_stats_yield");
        lb_pc_stats_yield_title = new QLabel(gb_pc_stats_yield);
        lb_pc_stats_yield_title->setObjectName("lb_pc_stats_yield_title");
        lb_pc_stats_yield_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_pc_stats_yield->addWidget(lb_pc_stats_yield_title);

        lb_pc_stats_yield_value = new QLabel(gb_pc_stats_yield);
        lb_pc_stats_yield_value->setObjectName("lb_pc_stats_yield_value");
        lb_pc_stats_yield_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:28px;font-weight:700;"));

        vl_pc_stats_yield->addWidget(lb_pc_stats_yield_value);

        pb_pc_stats_yield = new QProgressBar(gb_pc_stats_yield);
        pb_pc_stats_yield->setObjectName("pb_pc_stats_yield");
        pb_pc_stats_yield->setValue(84);

        vl_pc_stats_yield->addWidget(pb_pc_stats_yield);


        hl_pc_stats_cards->addWidget(gb_pc_stats_yield);

        gb_pc_stats_loss = new QGroupBox(tab_pc_stats);
        gb_pc_stats_loss->setObjectName("gb_pc_stats_loss");
        vl_pc_stats_loss = new QVBoxLayout(gb_pc_stats_loss);
        vl_pc_stats_loss->setObjectName("vl_pc_stats_loss");
        lb_pc_stats_loss_title = new QLabel(gb_pc_stats_loss);
        lb_pc_stats_loss_title->setObjectName("lb_pc_stats_loss_title");
        lb_pc_stats_loss_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_pc_stats_loss->addWidget(lb_pc_stats_loss_title);

        lb_pc_stats_loss_value = new QLabel(gb_pc_stats_loss);
        lb_pc_stats_loss_value->setObjectName("lb_pc_stats_loss_value");
        lb_pc_stats_loss_value->setStyleSheet(QString::fromUtf8("color:white;background:transparent;font-size:28px;font-weight:700;"));

        vl_pc_stats_loss->addWidget(lb_pc_stats_loss_value);

        lb_pc_stats_loss_hint = new QLabel(gb_pc_stats_loss);
        lb_pc_stats_loss_hint->setObjectName("lb_pc_stats_loss_hint");
        lb_pc_stats_loss_hint->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.65);background:transparent;"));

        vl_pc_stats_loss->addWidget(lb_pc_stats_loss_hint);


        hl_pc_stats_cards->addWidget(gb_pc_stats_loss);


        vl_pc_stats->addLayout(hl_pc_stats_cards);

        hl_pc_stats_body = new QHBoxLayout();
        hl_pc_stats_body->setSpacing(18);
        hl_pc_stats_body->setObjectName("hl_pc_stats_body");
        gb_pc_stats_materials = new QGroupBox(tab_pc_stats);
        gb_pc_stats_materials->setObjectName("gb_pc_stats_materials");
        vl_pc_stats_materials = new QVBoxLayout(gb_pc_stats_materials);
        vl_pc_stats_materials->setSpacing(12);
        vl_pc_stats_materials->setObjectName("vl_pc_stats_materials");
        hl_pc_stats_sort = new QHBoxLayout();
        hl_pc_stats_sort->setSpacing(10);
        hl_pc_stats_sort->setObjectName("hl_pc_stats_sort");
        lb_pc_stats_sort = new QLabel(gb_pc_stats_materials);
        lb_pc_stats_sort->setObjectName("lb_pc_stats_sort");
        lb_pc_stats_sort->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        hl_pc_stats_sort->addWidget(lb_pc_stats_sort);

        pc_stats_sort_by = new QComboBox(gb_pc_stats_materials);
        pc_stats_sort_by->addItem(QString());
        pc_stats_sort_by->addItem(QString());
        pc_stats_sort_by->addItem(QString());
        pc_stats_sort_by->setObjectName("pc_stats_sort_by");

        hl_pc_stats_sort->addWidget(pc_stats_sort_by);

        pc_stats_sort_order = new QComboBox(gb_pc_stats_materials);
        pc_stats_sort_order->addItem(QString());
        pc_stats_sort_order->addItem(QString());
        pc_stats_sort_order->setObjectName("pc_stats_sort_order");

        hl_pc_stats_sort->addWidget(pc_stats_sort_order);


        vl_pc_stats_materials->addLayout(hl_pc_stats_sort);

        lb_pc_stats_mat_1 = new QLabel(gb_pc_stats_materials);
        lb_pc_stats_mat_1->setObjectName("lb_pc_stats_mat_1");

        vl_pc_stats_materials->addWidget(lb_pc_stats_mat_1);

        pb_pc_stats_mat_1 = new QProgressBar(gb_pc_stats_materials);
        pb_pc_stats_mat_1->setObjectName("pb_pc_stats_mat_1");
        pb_pc_stats_mat_1->setValue(72);

        vl_pc_stats_materials->addWidget(pb_pc_stats_mat_1);

        lb_pc_stats_mat_2 = new QLabel(gb_pc_stats_materials);
        lb_pc_stats_mat_2->setObjectName("lb_pc_stats_mat_2");

        vl_pc_stats_materials->addWidget(lb_pc_stats_mat_2);

        pb_pc_stats_mat_2 = new QProgressBar(gb_pc_stats_materials);
        pb_pc_stats_mat_2->setObjectName("pb_pc_stats_mat_2");
        pb_pc_stats_mat_2->setValue(54);

        vl_pc_stats_materials->addWidget(pb_pc_stats_mat_2);

        lb_pc_stats_mat_3 = new QLabel(gb_pc_stats_materials);
        lb_pc_stats_mat_3->setObjectName("lb_pc_stats_mat_3");

        vl_pc_stats_materials->addWidget(lb_pc_stats_mat_3);

        pb_pc_stats_mat_3 = new QProgressBar(gb_pc_stats_materials);
        pb_pc_stats_mat_3->setObjectName("pb_pc_stats_mat_3");
        pb_pc_stats_mat_3->setValue(38);

        vl_pc_stats_materials->addWidget(pb_pc_stats_mat_3);

        pc_stats_material_chart_container = new QWidget(gb_pc_stats_materials);
        pc_stats_material_chart_container->setObjectName("pc_stats_material_chart_container");
        pc_stats_material_chart_container->setMinimumSize(QSize(0, 220));
        pc_stats_material_chart_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        vl_pc_stats_materials->addWidget(pc_stats_material_chart_container);


        hl_pc_stats_body->addWidget(gb_pc_stats_materials);

        gb_pc_stats_pipeline = new QGroupBox(tab_pc_stats);
        gb_pc_stats_pipeline->setObjectName("gb_pc_stats_pipeline");
        vl_pc_stats_pipeline = new QVBoxLayout(gb_pc_stats_pipeline);
        vl_pc_stats_pipeline->setSpacing(12);
        vl_pc_stats_pipeline->setObjectName("vl_pc_stats_pipeline");
        lb_pc_stats_pipeline_title = new QLabel(gb_pc_stats_pipeline);
        lb_pc_stats_pipeline_title->setObjectName("lb_pc_stats_pipeline_title");
        lb_pc_stats_pipeline_title->setStyleSheet(QString::fromUtf8("color:#a5b4fc;background:transparent;font-size:13px;font-weight:600;"));

        vl_pc_stats_pipeline->addWidget(lb_pc_stats_pipeline_title);

        pc_stats_trend_chart_container = new QWidget(gb_pc_stats_pipeline);
        pc_stats_trend_chart_container->setObjectName("pc_stats_trend_chart_container");
        pc_stats_trend_chart_container->setMinimumSize(QSize(0, 220));
        pc_stats_trend_chart_container->setStyleSheet(QString::fromUtf8("background:transparent;"));

        vl_pc_stats_pipeline->addWidget(pc_stats_trend_chart_container);

        txt_pc_stats_pipeline = new QTextEdit(gb_pc_stats_pipeline);
        txt_pc_stats_pipeline->setObjectName("txt_pc_stats_pipeline");
        txt_pc_stats_pipeline->setMinimumSize(QSize(0, 220));
        txt_pc_stats_pipeline->setReadOnly(true);

        vl_pc_stats_pipeline->addWidget(txt_pc_stats_pipeline);

        btn_pc_stats_refresh = new QPushButton(gb_pc_stats_pipeline);
        btn_pc_stats_refresh->setObjectName("btn_pc_stats_refresh");

        vl_pc_stats_pipeline->addWidget(btn_pc_stats_refresh);


        hl_pc_stats_body->addWidget(gb_pc_stats_pipeline);


        vl_pc_stats->addLayout(hl_pc_stats_body);

        tabWidget_pc->addTab(tab_pc_stats, QString());
        tab_pc_model_ai = new QWidget();
        tab_pc_model_ai->setObjectName("tab_pc_model_ai");
        vl_pc_model_ai = new QVBoxLayout(tab_pc_model_ai);
        vl_pc_model_ai->setObjectName("vl_pc_model_ai");
        vl_pc_model_ai->setContentsMargins(20, 20, 20, 20);
        tabWidget_pc_model_pages = new QTabWidget(tab_pc_model_ai);
        tabWidget_pc_model_pages->setObjectName("tabWidget_pc_model_pages");
        tab_pc_model_prepare = new QWidget();
        tab_pc_model_prepare->setObjectName("tab_pc_model_prepare");
        vl_pc_model_prepare_page = new QVBoxLayout(tab_pc_model_prepare);
        vl_pc_model_prepare_page->setSpacing(0);
        vl_pc_model_prepare_page->setObjectName("vl_pc_model_prepare_page");
        vl_pc_model_prepare_page->setContentsMargins(0, 0, 0, 0);
        hl_pc_model_top = new QHBoxLayout();
        hl_pc_model_top->setObjectName("hl_pc_model_top");
        gb_pc_model_source = new QGroupBox(tab_pc_model_prepare);
        gb_pc_model_source->setObjectName("gb_pc_model_source");
        vl_pc_model_source = new QVBoxLayout(gb_pc_model_source);
        vl_pc_model_source->setObjectName("vl_pc_model_source");
        lb_pc_model_dropzone = new QLabel(gb_pc_model_source);
        lb_pc_model_dropzone->setObjectName("lb_pc_model_dropzone");
        lb_pc_model_dropzone->setMinimumSize(QSize(0, 210));
        lb_pc_model_dropzone->setStyleSheet(QString::fromUtf8("border:2px dashed rgba(129,140,248,0.45);border-radius:16px;background:rgba(99,102,241,0.06);padding:24px;color:rgba(255,255,255,0.78);"));
        lb_pc_model_dropzone->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vl_pc_model_source->addWidget(lb_pc_model_dropzone);

        hl_pc_model_import = new QHBoxLayout();
        hl_pc_model_import->setObjectName("hl_pc_model_import");
        btn_pc_model_import = new QPushButton(gb_pc_model_source);
        btn_pc_model_import->setObjectName("btn_pc_model_import");

        hl_pc_model_import->addWidget(btn_pc_model_import);

        btn_pc_model_preview = new QPushButton(gb_pc_model_source);
        btn_pc_model_preview->setObjectName("btn_pc_model_preview");
        btn_pc_model_preview->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        hl_pc_model_import->addWidget(btn_pc_model_preview);


        vl_pc_model_source->addLayout(hl_pc_model_import);


        hl_pc_model_top->addWidget(gb_pc_model_source);

        gb_pc_model_settings = new QGroupBox(tab_pc_model_prepare);
        gb_pc_model_settings->setObjectName("gb_pc_model_settings");
        gl_pc_model_settings = new QGridLayout(gb_pc_model_settings);
        gl_pc_model_settings->setObjectName("gl_pc_model_settings");
        gl_pc_model_settings->setHorizontalSpacing(15);
        gl_pc_model_settings->setVerticalSpacing(10);
        lb_pc_model_selected = new QLabel(gb_pc_model_settings);
        lb_pc_model_selected->setObjectName("lb_pc_model_selected");

        gl_pc_model_settings->addWidget(lb_pc_model_selected, 0, 0, 1, 1);

        pc_ai_modele_source = new QComboBox(gb_pc_model_settings);
        pc_ai_modele_source->setObjectName("pc_ai_modele_source");

        gl_pc_model_settings->addWidget(pc_ai_modele_source, 0, 1, 1, 1);

        lb_pc_model_precision = new QLabel(gb_pc_model_settings);
        lb_pc_model_precision->setObjectName("lb_pc_model_precision");

        gl_pc_model_settings->addWidget(lb_pc_model_precision, 1, 0, 1, 1);

        pc_ai_precision = new QComboBox(gb_pc_model_settings);
        pc_ai_precision->addItem(QString());
        pc_ai_precision->addItem(QString());
        pc_ai_precision->addItem(QString());
        pc_ai_precision->setObjectName("pc_ai_precision");

        gl_pc_model_settings->addWidget(pc_ai_precision, 1, 1, 1, 1);

        lb_pc_model_tolerance = new QLabel(gb_pc_model_settings);
        lb_pc_model_tolerance->setObjectName("lb_pc_model_tolerance");

        gl_pc_model_settings->addWidget(lb_pc_model_tolerance, 2, 0, 1, 1);

        pc_ai_tolerance = new QDoubleSpinBox(gb_pc_model_settings);
        pc_ai_tolerance->setObjectName("pc_ai_tolerance");
        pc_ai_tolerance->setDecimals(2);
        pc_ai_tolerance->setMaximum(25.000000000000000);
        pc_ai_tolerance->setValue(1.500000000000000);

        gl_pc_model_settings->addWidget(pc_ai_tolerance, 2, 1, 1, 1);

        chk_pc_ai_edges = new QCheckBox(gb_pc_model_settings);
        chk_pc_ai_edges->setObjectName("chk_pc_ai_edges");
        chk_pc_ai_edges->setChecked(true);

        gl_pc_model_settings->addWidget(chk_pc_ai_edges, 3, 0, 1, 1);

        chk_pc_ai_merge = new QCheckBox(gb_pc_model_settings);
        chk_pc_ai_merge->setObjectName("chk_pc_ai_merge");
        chk_pc_ai_merge->setChecked(true);

        gl_pc_model_settings->addWidget(chk_pc_ai_merge, 3, 1, 1, 1);

        tabWidget_pc_generation_modes = new QTabWidget(gb_pc_model_settings);
        tabWidget_pc_generation_modes->setObjectName("tabWidget_pc_generation_modes");
        tab_pc_generation_manual = new QWidget();
        tab_pc_generation_manual->setObjectName("tab_pc_generation_manual");
        vl_pc_generation_manual = new QVBoxLayout(tab_pc_generation_manual);
        vl_pc_generation_manual->setObjectName("vl_pc_generation_manual");
        txt_pc_manual_notes = new QTextEdit(tab_pc_generation_manual);
        txt_pc_manual_notes->setObjectName("txt_pc_manual_notes");
        txt_pc_manual_notes->setMaximumSize(QSize(16777215, 16777215));

        vl_pc_generation_manual->addWidget(txt_pc_manual_notes);

        btn_pc_manual_generate = new QPushButton(tab_pc_generation_manual);
        btn_pc_manual_generate->setObjectName("btn_pc_manual_generate");

        vl_pc_generation_manual->addWidget(btn_pc_manual_generate);

        tabWidget_pc_generation_modes->addTab(tab_pc_generation_manual, QString());
        tab_pc_generation_auto = new QWidget();
        tab_pc_generation_auto->setObjectName("tab_pc_generation_auto");
        vl_pc_generation_auto = new QVBoxLayout(tab_pc_generation_auto);
        vl_pc_generation_auto->setObjectName("vl_pc_generation_auto");
        lb_pc_auto_hint = new QLabel(tab_pc_generation_auto);
        lb_pc_auto_hint->setObjectName("lb_pc_auto_hint");
        lb_pc_auto_hint->setStyleSheet(QString::fromUtf8("background:rgba(99,102,241,0.08);border:1px solid rgba(99,102,241,0.18);border-radius:12px;padding:14px;color:rgba(255,255,255,0.8);"));
        lb_pc_auto_hint->setWordWrap(true);

        vl_pc_generation_auto->addWidget(lb_pc_auto_hint);

        btn_pc_auto_generate = new QPushButton(tab_pc_generation_auto);
        btn_pc_auto_generate->setObjectName("btn_pc_auto_generate");

        vl_pc_generation_auto->addWidget(btn_pc_auto_generate);

        tabWidget_pc_generation_modes->addTab(tab_pc_generation_auto, QString());

        gl_pc_model_settings->addWidget(tabWidget_pc_generation_modes, 4, 0, 1, 2);


        hl_pc_model_top->addWidget(gb_pc_model_settings);


        vl_pc_model_prepare_page->addLayout(hl_pc_model_top);

        tabWidget_pc_model_pages->addTab(tab_pc_model_prepare, QString());
        tab_pc_model_draft = new QWidget();
        tab_pc_model_draft->setObjectName("tab_pc_model_draft");
        vl_pc_model_draft_page = new QVBoxLayout(tab_pc_model_draft);
        vl_pc_model_draft_page->setSpacing(0);
        vl_pc_model_draft_page->setObjectName("vl_pc_model_draft_page");
        vl_pc_model_draft_page->setContentsMargins(0, 0, 0, 0);
        gb_pc_model_result = new QGroupBox(tab_pc_model_draft);
        gb_pc_model_result->setObjectName("gb_pc_model_result");
        vl_pc_model_result = new QVBoxLayout(gb_pc_model_result);
        vl_pc_model_result->setObjectName("vl_pc_model_result");
        lb_pc_model_result_summary = new QLabel(gb_pc_model_result);
        lb_pc_model_result_summary->setObjectName("lb_pc_model_result_summary");
        lb_pc_model_result_summary->setStyleSheet(QString::fromUtf8("background:rgba(99,102,241,0.08);border:1px solid rgba(99,102,241,0.18);border-radius:12px;padding:14px;color:rgba(255,255,255,0.85);"));
        lb_pc_model_result_summary->setWordWrap(true);

        vl_pc_model_result->addWidget(lb_pc_model_result_summary);

        txt_pc_model_result = new QTextEdit(gb_pc_model_result);
        txt_pc_model_result->setObjectName("txt_pc_model_result");
        txt_pc_model_result->setReadOnly(true);

        vl_pc_model_result->addWidget(txt_pc_model_result);


        vl_pc_model_draft_page->addWidget(gb_pc_model_result);

        tabWidget_pc_model_pages->addTab(tab_pc_model_draft, QString());

        vl_pc_model_ai->addWidget(tabWidget_pc_model_pages);

        tabWidget_pc->addTab(tab_pc_model_ai, QString());
        tab_pc_nesting = new QWidget();
        tab_pc_nesting->setObjectName("tab_pc_nesting");
        vl_pc_nesting = new QVBoxLayout(tab_pc_nesting);
        vl_pc_nesting->setObjectName("vl_pc_nesting");
        vl_pc_nesting->setContentsMargins(20, 20, 20, 20);
        hl_pc_nesting_top = new QHBoxLayout();
        hl_pc_nesting_top->setObjectName("hl_pc_nesting_top");
        gb_pc_nesting_sheet = new QGroupBox(tab_pc_nesting);
        gb_pc_nesting_sheet->setObjectName("gb_pc_nesting_sheet");
        gl_pc_nesting_sheet = new QGridLayout(gb_pc_nesting_sheet);
        gl_pc_nesting_sheet->setObjectName("gl_pc_nesting_sheet");
        gl_pc_nesting_sheet->setHorizontalSpacing(15);
        gl_pc_nesting_sheet->setVerticalSpacing(10);
        chk_pc_nesting_save_scrap = new QCheckBox(gb_pc_nesting_sheet);
        chk_pc_nesting_save_scrap->setObjectName("chk_pc_nesting_save_scrap");
        chk_pc_nesting_save_scrap->setChecked(true);

        gl_pc_nesting_sheet->addWidget(chk_pc_nesting_save_scrap, 4, 1, 1, 1);

        pc_sheet_gap = new QDoubleSpinBox(gb_pc_nesting_sheet);
        pc_sheet_gap->setObjectName("pc_sheet_gap");
        pc_sheet_gap->setMaximum(100.000000000000000);
        pc_sheet_gap->setValue(3.000000000000000);

        gl_pc_nesting_sheet->addWidget(pc_sheet_gap, 2, 1, 1, 1);

        lb_pc_nesting_width = new QLabel(gb_pc_nesting_sheet);
        lb_pc_nesting_width->setObjectName("lb_pc_nesting_width");

        gl_pc_nesting_sheet->addWidget(lb_pc_nesting_width, 1, 0, 1, 1);

        pc_sheet_largeur = new QDoubleSpinBox(gb_pc_nesting_sheet);
        pc_sheet_largeur->setObjectName("pc_sheet_largeur");
        pc_sheet_largeur->setMaximum(999999.000000000000000);
        pc_sheet_largeur->setValue(2070.000000000000000);

        gl_pc_nesting_sheet->addWidget(pc_sheet_largeur, 1, 1, 1, 1);

        pc_sheet_longueur = new QDoubleSpinBox(gb_pc_nesting_sheet);
        pc_sheet_longueur->setObjectName("pc_sheet_longueur");
        pc_sheet_longueur->setMaximum(999999.000000000000000);
        pc_sheet_longueur->setValue(2800.000000000000000);

        gl_pc_nesting_sheet->addWidget(pc_sheet_longueur, 0, 1, 1, 1);

        lb_pc_nesting_length = new QLabel(gb_pc_nesting_sheet);
        lb_pc_nesting_length->setObjectName("lb_pc_nesting_length");

        gl_pc_nesting_sheet->addWidget(lb_pc_nesting_length, 0, 0, 1, 1);

        pc_nesting_algo = new QComboBox(gb_pc_nesting_sheet);
        pc_nesting_algo->addItem(QString());
        pc_nesting_algo->addItem(QString());
        pc_nesting_algo->addItem(QString());
        pc_nesting_algo->setObjectName("pc_nesting_algo");

        gl_pc_nesting_sheet->addWidget(pc_nesting_algo, 3, 1, 1, 1);

        chk_pc_nesting_rotate = new QCheckBox(gb_pc_nesting_sheet);
        chk_pc_nesting_rotate->setObjectName("chk_pc_nesting_rotate");
        chk_pc_nesting_rotate->setChecked(true);

        gl_pc_nesting_sheet->addWidget(chk_pc_nesting_rotate, 4, 0, 1, 1);

        lb_pc_nesting_algo = new QLabel(gb_pc_nesting_sheet);
        lb_pc_nesting_algo->setObjectName("lb_pc_nesting_algo");

        gl_pc_nesting_sheet->addWidget(lb_pc_nesting_algo, 3, 0, 1, 1);

        lb_pc_nesting_gap = new QLabel(gb_pc_nesting_sheet);
        lb_pc_nesting_gap->setObjectName("lb_pc_nesting_gap");

        gl_pc_nesting_sheet->addWidget(lb_pc_nesting_gap, 2, 0, 1, 1);


        hl_pc_nesting_top->addWidget(gb_pc_nesting_sheet);

        gb_pc_nesting_metrics = new QGroupBox(tab_pc_nesting);
        gb_pc_nesting_metrics->setObjectName("gb_pc_nesting_metrics");
        vl_pc_nesting_metrics = new QVBoxLayout(gb_pc_nesting_metrics);
        vl_pc_nesting_metrics->setObjectName("vl_pc_nesting_metrics");
        lb_pc_nesting_eff = new QLabel(gb_pc_nesting_metrics);
        lb_pc_nesting_eff->setObjectName("lb_pc_nesting_eff");

        vl_pc_nesting_metrics->addWidget(lb_pc_nesting_eff);

        pb_pc_nesting_eff = new QProgressBar(gb_pc_nesting_metrics);
        pb_pc_nesting_eff->setObjectName("pb_pc_nesting_eff");
        pb_pc_nesting_eff->setValue(91);

        vl_pc_nesting_metrics->addWidget(pb_pc_nesting_eff);

        lb_pc_nesting_scrap = new QLabel(gb_pc_nesting_metrics);
        lb_pc_nesting_scrap->setObjectName("lb_pc_nesting_scrap");

        vl_pc_nesting_metrics->addWidget(lb_pc_nesting_scrap);

        pb_pc_nesting_scrap = new QProgressBar(gb_pc_nesting_metrics);
        pb_pc_nesting_scrap->setObjectName("pb_pc_nesting_scrap");
        pb_pc_nesting_scrap->setValue(63);

        vl_pc_nesting_metrics->addWidget(pb_pc_nesting_scrap);

        lb_pc_nesting_summary = new QLabel(gb_pc_nesting_metrics);
        lb_pc_nesting_summary->setObjectName("lb_pc_nesting_summary");
        lb_pc_nesting_summary->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.15);border-radius:12px;padding:12px;color:rgba(255,255,255,0.78);"));
        lb_pc_nesting_summary->setWordWrap(true);

        vl_pc_nesting_metrics->addWidget(lb_pc_nesting_summary);

        btn_pc_nesting_run = new QPushButton(gb_pc_nesting_metrics);
        btn_pc_nesting_run->setObjectName("btn_pc_nesting_run");

        vl_pc_nesting_metrics->addWidget(btn_pc_nesting_run);


        hl_pc_nesting_top->addWidget(gb_pc_nesting_metrics);


        vl_pc_nesting->addLayout(hl_pc_nesting_top);

        hl_pc_nesting_bottom = new QHBoxLayout();
        hl_pc_nesting_bottom->setObjectName("hl_pc_nesting_bottom");
        gb_pc_nesting_canvas = new QGroupBox(tab_pc_nesting);
        gb_pc_nesting_canvas->setObjectName("gb_pc_nesting_canvas");
        vl_pc_nesting_canvas = new QVBoxLayout(gb_pc_nesting_canvas);
        vl_pc_nesting_canvas->setObjectName("vl_pc_nesting_canvas");
        lb_pc_nesting_canvas = new QLabel(gb_pc_nesting_canvas);
        lb_pc_nesting_canvas->setObjectName("lb_pc_nesting_canvas");
        lb_pc_nesting_canvas->setMinimumSize(QSize(0, 250));
        lb_pc_nesting_canvas->setStyleSheet(QString::fromUtf8("border:2px dashed rgba(129,140,248,0.38);border-radius:16px;background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 rgba(99,102,241,0.08),stop:1 rgba(30,41,59,0.32));padding:24px;color:rgba(255,255,255,0.75);"));
        lb_pc_nesting_canvas->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vl_pc_nesting_canvas->addWidget(lb_pc_nesting_canvas);


        hl_pc_nesting_bottom->addWidget(gb_pc_nesting_canvas);

        gb_pc_nesting_actions = new QGroupBox(tab_pc_nesting);
        gb_pc_nesting_actions->setObjectName("gb_pc_nesting_actions");
        vl_pc_nesting_actions = new QVBoxLayout(gb_pc_nesting_actions);
        vl_pc_nesting_actions->setObjectName("vl_pc_nesting_actions");
        btn_pc_nesting_export_pdf = new QPushButton(gb_pc_nesting_actions);
        btn_pc_nesting_export_pdf->setObjectName("btn_pc_nesting_export_pdf");

        vl_pc_nesting_actions->addWidget(btn_pc_nesting_export_pdf);

        vs_pc_nesting_actions = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vl_pc_nesting_actions->addItem(vs_pc_nesting_actions);

        btn_pc_nesting_export_csv = new QPushButton(gb_pc_nesting_actions);
        btn_pc_nesting_export_csv->setObjectName("btn_pc_nesting_export_csv");
        btn_pc_nesting_export_csv->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        vl_pc_nesting_actions->addWidget(btn_pc_nesting_export_csv);

        txt_pc_nesting_log = new QTextEdit(gb_pc_nesting_actions);
        txt_pc_nesting_log->setObjectName("txt_pc_nesting_log");
        txt_pc_nesting_log->setMaximumSize(QSize(16777215, 16777215));
        txt_pc_nesting_log->setReadOnly(true);

        vl_pc_nesting_actions->addWidget(txt_pc_nesting_log);


        hl_pc_nesting_bottom->addWidget(gb_pc_nesting_actions);


        vl_pc_nesting->addLayout(hl_pc_nesting_bottom);

        tabWidget_pc->addTab(tab_pc_nesting, QString());
        tab_pc_manual_cut = new QWidget();
        tab_pc_manual_cut->setObjectName("tab_pc_manual_cut");
        vl_pc_manual_cut = new QVBoxLayout(tab_pc_manual_cut);
        vl_pc_manual_cut->setObjectName("vl_pc_manual_cut");
        vl_pc_manual_cut->setContentsMargins(20, 20, 20, 20);
        hl_pc_manual_top = new QHBoxLayout();
        hl_pc_manual_top->setObjectName("hl_pc_manual_top");
        gb_pc_manual_source = new QGroupBox(tab_pc_manual_cut);
        gb_pc_manual_source->setObjectName("gb_pc_manual_source");
        gl_pc_manual_source = new QGridLayout(gb_pc_manual_source);
        gl_pc_manual_source->setObjectName("gl_pc_manual_source");
        gl_pc_manual_source->setHorizontalSpacing(15);
        gl_pc_manual_source->setVerticalSpacing(10);
        lb_pc_manual_modele = new QLabel(gb_pc_manual_source);
        lb_pc_manual_modele->setObjectName("lb_pc_manual_modele");

        gl_pc_manual_source->addWidget(lb_pc_manual_modele, 0, 0, 1, 1);

        pc_manual_modele = new QComboBox(gb_pc_manual_source);
        pc_manual_modele->setObjectName("pc_manual_modele");

        gl_pc_manual_source->addWidget(pc_manual_modele, 0, 1, 1, 1);

        lb_pc_manual_bois = new QLabel(gb_pc_manual_source);
        lb_pc_manual_bois->setObjectName("lb_pc_manual_bois");

        gl_pc_manual_source->addWidget(lb_pc_manual_bois, 1, 0, 1, 1);

        pc_manual_bois = new QComboBox(gb_pc_manual_source);
        pc_manual_bois->setObjectName("pc_manual_bois");

        gl_pc_manual_source->addWidget(pc_manual_bois, 1, 1, 1, 1);

        lb_pc_manual_passes = new QLabel(gb_pc_manual_source);
        lb_pc_manual_passes->setObjectName("lb_pc_manual_passes");

        gl_pc_manual_source->addWidget(lb_pc_manual_passes, 2, 0, 1, 1);

        pc_manual_passes = new QSpinBox(gb_pc_manual_source);
        pc_manual_passes->setObjectName("pc_manual_passes");
        pc_manual_passes->setMinimum(1);
        pc_manual_passes->setMaximum(20);
        pc_manual_passes->setValue(4);

        gl_pc_manual_source->addWidget(pc_manual_passes, 2, 1, 1, 1);

        chk_pc_manual_keep_offcuts = new QCheckBox(gb_pc_manual_source);
        chk_pc_manual_keep_offcuts->setObjectName("chk_pc_manual_keep_offcuts");
        chk_pc_manual_keep_offcuts->setChecked(true);

        gl_pc_manual_source->addWidget(chk_pc_manual_keep_offcuts, 3, 0, 1, 1);

        chk_pc_manual_axis_lock = new QCheckBox(gb_pc_manual_source);
        chk_pc_manual_axis_lock->setObjectName("chk_pc_manual_axis_lock");

        gl_pc_manual_source->addWidget(chk_pc_manual_axis_lock, 3, 1, 1, 1);


        hl_pc_manual_top->addWidget(gb_pc_manual_source);

        gb_pc_manual_canvas = new QGroupBox(tab_pc_manual_cut);
        gb_pc_manual_canvas->setObjectName("gb_pc_manual_canvas");
        vl_pc_manual_canvas = new QVBoxLayout(gb_pc_manual_canvas);
        vl_pc_manual_canvas->setObjectName("vl_pc_manual_canvas");
        lb_pc_manual_canvas = new QLabel(gb_pc_manual_canvas);
        lb_pc_manual_canvas->setObjectName("lb_pc_manual_canvas");
        lb_pc_manual_canvas->setMinimumSize(QSize(0, 220));
        lb_pc_manual_canvas->setStyleSheet(QString::fromUtf8("border:2px dashed rgba(129,140,248,0.38);border-radius:16px;background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 rgba(99,102,241,0.07),stop:1 rgba(15,23,42,0.4));padding:22px;color:rgba(255,255,255,0.78);"));
        lb_pc_manual_canvas->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vl_pc_manual_canvas->addWidget(lb_pc_manual_canvas);


        hl_pc_manual_top->addWidget(gb_pc_manual_canvas);


        vl_pc_manual_cut->addLayout(hl_pc_manual_top);

        gb_pc_manual_plan = new QGroupBox(tab_pc_manual_cut);
        gb_pc_manual_plan->setObjectName("gb_pc_manual_plan");
        vl_pc_manual_plan = new QVBoxLayout(gb_pc_manual_plan);
        vl_pc_manual_plan->setObjectName("vl_pc_manual_plan");
        txt_pc_manual_cut_plan = new QTextEdit(gb_pc_manual_plan);
        txt_pc_manual_cut_plan->setObjectName("txt_pc_manual_cut_plan");

        vl_pc_manual_plan->addWidget(txt_pc_manual_cut_plan);

        hl_pc_manual_actions = new QHBoxLayout();
        hl_pc_manual_actions->setObjectName("hl_pc_manual_actions");
        btn_pc_manual_add_step = new QPushButton(gb_pc_manual_plan);
        btn_pc_manual_add_step->setObjectName("btn_pc_manual_add_step");

        hl_pc_manual_actions->addWidget(btn_pc_manual_add_step);

        btn_pc_manual_validate = new QPushButton(gb_pc_manual_plan);
        btn_pc_manual_validate->setObjectName("btn_pc_manual_validate");

        hl_pc_manual_actions->addWidget(btn_pc_manual_validate);

        btn_pc_manual_export = new QPushButton(gb_pc_manual_plan);
        btn_pc_manual_export->setObjectName("btn_pc_manual_export");
        btn_pc_manual_export->setStyleSheet(QString::fromUtf8("background:rgba(255,255,255,0.08);border:1px solid rgba(99,102,241,0.2);"));

        hl_pc_manual_actions->addWidget(btn_pc_manual_export);


        vl_pc_manual_plan->addLayout(hl_pc_manual_actions);


        vl_pc_manual_cut->addWidget(gb_pc_manual_plan);

        tabWidget_pc->addTab(tab_pc_manual_cut, QString());
        stackedWidget->addWidget(Ppieces);
        Psettings = new QWidget();
        Psettings->setObjectName("Psettings");
        c_Pset = new QWidget(Psettings);
        c_Pset->setObjectName("c_Pset");
        c_Pset->setGeometry(QRect(0, 0, 1201, 731));
        c_Pset->setStyleSheet(QString::fromUtf8("QLabel{color:rgba(255,255,255,0.92);font-weight:500;font-family:'Segoe UI',sans-serif;}QWidget{background-color:#11112a;color:rgba(255,255,255,0.9);font-family:'Segoe UI',sans-serif;font-size:14px;}QGroupBox{border:1px solid rgba(99,102,241,0.15);border-radius:14px;margin-top:14px;padding:18px;background-color:rgba(18,18,42,0.8);}QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;padding:0 8px;color:#818cf8;font-weight:700;}QLineEdit{background-color:rgba(255,255,255,0.05);border:1px solid rgba(99,102,241,0.2);border-radius:8px;padding:8px 12px;color:white;}QLineEdit:focus{border:1px solid #6366f1;background-color:rgba(99,102,241,0.08);}QCheckBox{color:rgba(255,255,255,0.85);spacing:10px;}QCheckBox::indicator{width:18px;height:18px;border-radius:4px;}QCheckBox::indicator:unchecked{border:2px solid rgba(99,102,241,0.5);background:transparent;}QCheckBox::indicator:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #6366f1,stop:1 #818cf8);border:2px solid #6366f1;}QPushButton{backgr"
                        "ound:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #4f46e5,stop:1 #6366f1);color:white;border-radius:10px;padding:10px 20px;font-weight:600;border:none;}QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #6366f1,stop:1 #818cf8);}"));
        t_set = new QLabel(c_Pset);
        t_set->setObjectName("t_set");
        t_set->setGeometry(QRect(30, 15, 400, 35));
        t_set->setFont(font);
        t_set->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,0.95);background:transparent;"));
        gf_set = new QGroupBox(c_Pset);
        gf_set->setObjectName("gf_set");
        gf_set->setGeometry(QRect(30, 60, 550, 280));
        fl_set = new QGridLayout(gf_set);
        fl_set->setObjectName("fl_set");
        fl_set->setHorizontalSpacing(15);
        fl_set->setVerticalSpacing(10);
        ls0 = new QLabel(gf_set);
        ls0->setObjectName("ls0");

        fl_set->addWidget(ls0, 0, 0, 1, 1);

        set_host = new QLineEdit(gf_set);
        set_host->setObjectName("set_host");

        fl_set->addWidget(set_host, 0, 1, 1, 1);

        ls1 = new QLabel(gf_set);
        ls1->setObjectName("ls1");

        fl_set->addWidget(ls1, 1, 0, 1, 1);

        set_port = new QLineEdit(gf_set);
        set_port->setObjectName("set_port");

        fl_set->addWidget(set_port, 1, 1, 1, 1);

        ls2 = new QLabel(gf_set);
        ls2->setObjectName("ls2");

        fl_set->addWidget(ls2, 2, 0, 1, 1);

        set_db = new QLineEdit(gf_set);
        set_db->setObjectName("set_db");

        fl_set->addWidget(set_db, 2, 1, 1, 1);

        ls3 = new QLabel(gf_set);
        ls3->setObjectName("ls3");

        fl_set->addWidget(ls3, 3, 0, 1, 1);

        set_user = new QLineEdit(gf_set);
        set_user->setObjectName("set_user");

        fl_set->addWidget(set_user, 3, 1, 1, 1);

        ls4 = new QLabel(gf_set);
        ls4->setObjectName("ls4");

        fl_set->addWidget(ls4, 4, 0, 1, 1);

        set_pass = new QLineEdit(gf_set);
        set_pass->setObjectName("set_pass");
        set_pass->setEchoMode(QLineEdit::EchoMode::Password);

        fl_set->addWidget(set_pass, 4, 1, 1, 1);

        bb_set = new QWidget(c_Pset);
        bb_set->setObjectName("bb_set");
        bb_set->setGeometry(QRect(30, 350, 550, 50));
        bb_set->setStyleSheet(QString::fromUtf8("background:transparent;"));
        bl_set = new QHBoxLayout(bb_set);
        bl_set->setObjectName("bl_set");
        btn_test_conn = new QPushButton(bb_set);
        btn_test_conn->setObjectName("btn_test_conn");

        bl_set->addWidget(btn_test_conn);

        btn_save_settings = new QPushButton(bb_set);
        btn_save_settings->setObjectName("btn_save_settings");

        bl_set->addWidget(btn_save_settings);

        gf_app = new QGroupBox(c_Pset);
        gf_app->setObjectName("gf_app");
        gf_app->setGeometry(QRect(600, 60, 550, 200));
        al_set = new QVBoxLayout(gf_app);
        al_set->setObjectName("al_set");
        ttsenable = new QCheckBox(gf_app);
        ttsenable->setObjectName("ttsenable");

        al_set->addWidget(ttsenable);

        qrenable = new QCheckBox(gf_app);
        qrenable->setObjectName("qrenable");

        al_set->addWidget(qrenable);

        stackedWidget->addWidget(Psettings);

        gridLayout_4->addWidget(stackedWidget, 1, 2, 1, 1);

        smartcarpentry->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(smartcarpentry);
        statusbar->setObjectName("statusbar");
        smartcarpentry->setStatusBar(statusbar);

        retranslateUi(smartcarpentry);
        QObject::connect(pushButton_17, &QPushButton::toggled, icon_only_widget, &QWidget::setVisible);
        QObject::connect(pbmodele, &QPushButton::toggled, pushButton_6, &QPushButton::setChecked);
        QObject::connect(pbsettings, &QPushButton::toggled, pushButton_7, &QPushButton::setChecked);
        QObject::connect(pushButton_10, &QPushButton::toggled, pushButton_8, &QPushButton::setChecked);
        QObject::connect(pbpieces, &QPushButton::toggled, pushButton_9, &QPushButton::setChecked);
        QObject::connect(pbfabrication, &QPushButton::toggled, pushButton_5, &QPushButton::setChecked);
        QObject::connect(pushButton_17, &QPushButton::toggled, icon_text_widget, &QWidget::setHidden);
        QObject::connect(pbemployee, &QPushButton::toggled, pushButton_4, &QPushButton::setChecked);
        QObject::connect(pbbois, &QPushButton::toggled, pushButton_3, &QPushButton::setChecked);

        stackedWidget->setCurrentIndex(5);
        tabWidget_emp->setCurrentIndex(5);
        tabWidget_bois->setCurrentIndex(1);
        tabWidget_fab->setCurrentIndex(1);
        tabWidget_mod->setCurrentIndex(0);
        tabWidget_pc->setCurrentIndex(4);
        tabWidget_pc_model_pages->setCurrentIndex(0);
        tabWidget_pc_generation_modes->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(smartcarpentry);
    } // setupUi

    void retranslateUi(QMainWindow *smartcarpentry)
    {
        smartcarpentry->setWindowTitle(QCoreApplication::translate("smartcarpentry", "smartcarpentry", nullptr));
        label_6->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        pushButton_9->setText(QString());
        pushButton_7->setText(QString());
        pushButton_8->setText(QString());
        label_7->setText(QString());
        label_3->setText(QCoreApplication::translate("smartcarpentry", "<html><head/><body><p><span style=\" font-size:14pt;\">Woodify</span></p></body></html>", nullptr));
        pbbois->setText(QString());
        pbemployee->setText(QString());
        pbfabrication->setText(QString());
        pbmodele->setText(QString());
        pbpieces->setText(QString());
        pbsettings->setText(QString());
        pushButton_10->setText(QString());
        label_5->setText(QCoreApplication::translate("smartcarpentry", "<html><head/><body><p align=\"center\">Bienvenue sur votre espace</p></body></html>", nullptr));
        pushButton_17->setText(QString());
        label_4->setText(QCoreApplication::translate("smartcarpentry", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:700;\">Hello, Mrad</span></p></body></html>", nullptr));
        Profil_photo->setText(QString());
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher ici...", nullptr));
        t_emp->setText(QCoreApplication::translate("smartcarpentry", "Gestion des Employ?s", nullptr));
        search_emp->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher un employ?...", nullptr));
        lb_filter_role->setText(QCoreApplication::translate("smartcarpentry", "R?le :", nullptr));
        combo_filter_role->setItemText(0, QCoreApplication::translate("smartcarpentry", "Tous", nullptr));
        combo_filter_role->setItemText(1, QCoreApplication::translate("smartcarpentry", "Admin", nullptr));
        combo_filter_role->setItemText(2, QCoreApplication::translate("smartcarpentry", "Designer", nullptr));
        combo_filter_role->setItemText(3, QCoreApplication::translate("smartcarpentry", "CHEF_ATELIER", nullptr));
        combo_filter_role->setItemText(4, QCoreApplication::translate("smartcarpentry", "Ing?nieur", nullptr));
        combo_filter_role->setItemText(5, QCoreApplication::translate("smartcarpentry", "RH", nullptr));
        combo_filter_role->setItemText(6, QCoreApplication::translate("smartcarpentry", "Menuisier", nullptr));

        btn_emp_action_export->setText(QCoreApplication::translate("smartcarpentry", "Exporter PDF", nullptr));
        btn_emp_action_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser", nullptr));
        btn_emp_page_prev->setText(QCoreApplication::translate("smartcarpentry", "?", nullptr));
        btn_emp_page_next->setText(QCoreApplication::translate("smartcarpentry", "?", nullptr));
        lb_emp_pagination->setText(QCoreApplication::translate("smartcarpentry", "Affichage 0-0 sur 0 employ?s", nullptr));
        btn_emp_list_modify->setText(QCoreApplication::translate("smartcarpentry", "Ajouter un employ?", nullptr));
        btn_emp_list_delete->setText(QCoreApplication::translate("smartcarpentry", "Export PDF", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_list), QCoreApplication::translate("smartcarpentry", "Liste des employ?s", nullptr));
        gf_emp->setTitle(QCoreApplication::translate("smartcarpentry", "  Formulaire employ?", nullptr));
        le1->setText(QCoreApplication::translate("smartcarpentry", "CIN", nullptr));
        emp_cin->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "CIN unique", nullptr));
        le2->setText(QCoreApplication::translate("smartcarpentry", "Email", nullptr));
        emp_mail->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "email@example.com", nullptr));
        le3->setText(QCoreApplication::translate("smartcarpentry", "Mot de passe", nullptr));
        emp_password->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Mot de passe", nullptr));
        le4->setText(QCoreApplication::translate("smartcarpentry", "Nom", nullptr));
        emp_nom->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Nom de famille", nullptr));
        le5->setText(QCoreApplication::translate("smartcarpentry", "Prenom", nullptr));
        emp_prenom->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Prenom", nullptr));
        le6->setText(QCoreApplication::translate("smartcarpentry", "Date Naissance", nullptr));
        le7->setText(QCoreApplication::translate("smartcarpentry", "Role", nullptr));
        emp_role->setItemText(0, QCoreApplication::translate("smartcarpentry", "Admin", nullptr));
        emp_role->setItemText(1, QCoreApplication::translate("smartcarpentry", "Designer", nullptr));
        emp_role->setItemText(2, QCoreApplication::translate("smartcarpentry", "CHEF_ATELIER", nullptr));
        emp_role->setItemText(3, QCoreApplication::translate("smartcarpentry", "Ing?nieur", nullptr));
        emp_role->setItemText(4, QCoreApplication::translate("smartcarpentry", "RH", nullptr));
        emp_role->setItemText(5, QCoreApplication::translate("smartcarpentry", "Menuisier", nullptr));

        le10->setText(QCoreApplication::translate("smartcarpentry", "Photo Profil", nullptr));
        emp_face_id->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Chemin de la photo (FACE_ID)", nullptr));
        btn_emp_photo->setText(QCoreApplication::translate("smartcarpentry", "Choisir...", nullptr));
        btn_emp_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter", nullptr));
        btn_emp_edit->setText(QCoreApplication::translate("smartcarpentry", "Modifier", nullptr));
        btn_emp_del->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_emp_clr->setText(QCoreApplication::translate("smartcarpentry", "Vider", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_form), QCoreApplication::translate("smartcarpentry", "Ajouter / Modifier un employ?", nullptr));
        gb_affect_emp->setTitle(QCoreApplication::translate("smartcarpentry", "  Employ?s", nullptr));
        search_affect_emp->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher un employ?...", nullptr));
        gb_affect_fab->setTitle(QCoreApplication::translate("smartcarpentry", "  T?ches de Fabrication", nullptr));
        search_affect_fab->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher une fabrication...", nullptr));
        btn_emp_affect->setText(QCoreApplication::translate("smartcarpentry", "Affecter", nullptr));
        btn_emp_unaffect->setText(QCoreApplication::translate("smartcarpentry", "D?saffecter", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_affectation), QCoreApplication::translate("smartcarpentry", "Affectation T?ches", nullptr));
        gb_emp_stats_total->setTitle(QCoreApplication::translate("smartcarpentry", "  Total Employ?s", nullptr));
        lb_emp_stats_total_title->setText(QCoreApplication::translate("smartcarpentry", "Effectif total", nullptr));
        lb_emp_stats_total_value->setText(QCoreApplication::translate("smartcarpentry", "0", nullptr));
        lb_emp_stats_total_hint->setText(QCoreApplication::translate("smartcarpentry", "0 affect?s | 0 non affect?s", nullptr));
        gb_emp_stats_affect->setTitle(QCoreApplication::translate("smartcarpentry", "  Taux d'Affectation", nullptr));
        lb_emp_stats_affect_title->setText(QCoreApplication::translate("smartcarpentry", "Employ?s affect?s", nullptr));
        lb_emp_stats_affected_value->setText(QCoreApplication::translate("smartcarpentry", "0%", nullptr));
        gb_emp_stats_toprole->setTitle(QCoreApplication::translate("smartcarpentry", "  R?le le plus repr?sent?", nullptr));
        lb_emp_stats_toprole_title->setText(QCoreApplication::translate("smartcarpentry", "R?le dominant", nullptr));
        lb_emp_stats_toprole_value->setText(QCoreApplication::translate("smartcarpentry", "?", nullptr));
        lb_emp_stats_toprole_hint->setText(QCoreApplication::translate("smartcarpentry", "0 employ?(s)", nullptr));
        gb_emp_stats_roles->setTitle(QCoreApplication::translate("smartcarpentry", "  R?partition par r?le", nullptr));
        lb_emp_stats_role_1->setText(QCoreApplication::translate("smartcarpentry", "Aucune donn?e", nullptr));
        lb_emp_stats_role_2->setText(QCoreApplication::translate("smartcarpentry", "Aucune donn?e", nullptr));
        lb_emp_stats_role_3->setText(QCoreApplication::translate("smartcarpentry", "Aucune donn?e", nullptr));
        gb_emp_stats_pipeline->setTitle(QCoreApplication::translate("smartcarpentry", "  Vue d'ensemble de l'?quipe", nullptr));
        lb_emp_stats_pipeline_title->setText(QCoreApplication::translate("smartcarpentry", "Affectations par fabrication", nullptr));
        txt_emp_stats_pipeline->setPlainText(QCoreApplication::translate("smartcarpentry", "Chargement des statistiques...", nullptr));
        btn_emp_stats_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser les statistiques", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_stats), QCoreApplication::translate("smartcarpentry", "Statistiques", nullptr));
        gb_emp_knn_inputs->setTitle(QCoreApplication::translate("smartcarpentry", "  Param?tres de la t?che", nullptr));
        lb_emp_knn_task_x->setText(QCoreApplication::translate("smartcarpentry", "Axe X (t?che)", nullptr));
        lb_emp_knn_task_y->setText(QCoreApplication::translate("smartcarpentry", "Axe Y (t?che)", nullptr));
        lb_emp_knn_k->setText(QCoreApplication::translate("smartcarpentry", "Voisins K", nullptr));
        lb_emp_knn_role->setText(QCoreApplication::translate("smartcarpentry", "Filtre r?le", nullptr));
        emp_knn_role->setItemText(0, QCoreApplication::translate("smartcarpentry", "Tous", nullptr));
        emp_knn_role->setItemText(1, QCoreApplication::translate("smartcarpentry", "Admin", nullptr));
        emp_knn_role->setItemText(2, QCoreApplication::translate("smartcarpentry", "Designer", nullptr));
        emp_knn_role->setItemText(3, QCoreApplication::translate("smartcarpentry", "CHEF_ATELIER", nullptr));
        emp_knn_role->setItemText(4, QCoreApplication::translate("smartcarpentry", "Ing?nieur", nullptr));
        emp_knn_role->setItemText(5, QCoreApplication::translate("smartcarpentry", "RH", nullptr));
        emp_knn_role->setItemText(6, QCoreApplication::translate("smartcarpentry", "Menuisier", nullptr));

        lb_emp_knn_source_title->setText(QCoreApplication::translate("smartcarpentry", "Source de competences automatiques", nullptr));
        lb_emp_knn_source_type->setText(QCoreApplication::translate("smartcarpentry", "Type de source", nullptr));
        emp_knn_source_type->setItemText(0, QCoreApplication::translate("smartcarpentry", "Manuel", nullptr));
        emp_knn_source_type->setItemText(1, QCoreApplication::translate("smartcarpentry", "Fabrication", nullptr));
        emp_knn_source_type->setItemText(2, QCoreApplication::translate("smartcarpentry", "Phase", nullptr));

        lb_emp_knn_source_id->setText(QCoreApplication::translate("smartcarpentry", "ID de la source", nullptr));
        btn_emp_knn_run->setText(QCoreApplication::translate("smartcarpentry", "Lancer KNN", nullptr));
        lb_emp_knn_summary->setText(QCoreApplication::translate("smartcarpentry", "Ex?cutez KNN pour obtenir les meilleurs profils.", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_knn), QCoreApplication::translate("smartcarpentry", "Competences KNN", nullptr));
        gb_training_employee_select->setTitle(QCoreApplication::translate("smartcarpentry", "  S\303\251lectionner un employ\303\251", nullptr));
        combo_training_employee->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Choisir un employ\303\251...", nullptr));
        btn_training_analyze->setText(QCoreApplication::translate("smartcarpentry", "Analyser formation", nullptr));
        btn_training_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser", nullptr));
        gb_training_profile->setTitle(QCoreApplication::translate("smartcarpentry", "  Profil de l'employ\303\251", nullptr));
        lb_training_role->setText(QCoreApplication::translate("smartcarpentry", "R\303\264le:", nullptr));
        lb_training_role_value->setText(QCoreApplication::translate("smartcarpentry", "-", nullptr));
        lb_training_level->setText(QCoreApplication::translate("smartcarpentry", "Niveau:", nullptr));
        lb_training_level_value->setText(QCoreApplication::translate("smartcarpentry", "-", nullptr));
        lb_training_seniority->setText(QCoreApplication::translate("smartcarpentry", "Anciennet\303\251:", nullptr));
        lb_training_seniority_value->setText(QCoreApplication::translate("smartcarpentry", "-", nullptr));
        lb_training_skills_count->setText(QCoreApplication::translate("smartcarpentry", "Comp\303\251tences:", nullptr));
        lb_training_skills_count_value->setText(QCoreApplication::translate("smartcarpentry", "-", nullptr));
        gb_training_recommendations->setTitle(QCoreApplication::translate("smartcarpentry", "  Formations recommand\303\251es", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tw_training_recommendations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("smartcarpentry", "Formation", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tw_training_recommendations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("smartcarpentry", "Raison", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tw_training_recommendations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("smartcarpentry", "Priorit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tw_training_recommendations->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("smartcarpentry", "Dur\303\251e est.", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tw_training_recommendations->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("smartcarpentry", "Statut", nullptr));
        gb_training_current->setTitle(QCoreApplication::translate("smartcarpentry", "  Formations compl\303\251t\303\251es", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tw_training_completed->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("smartcarpentry", "Formation", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tw_training_completed->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("smartcarpentry", "Date compl\303\251t\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tw_training_completed->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("smartcarpentry", "Certificat", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tw_training_completed->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("smartcarpentry", "Acci\303\263n", nullptr));
        tabWidget_emp->setTabText(tabWidget_emp->indexOf(tab_emp_training), QCoreApplication::translate("smartcarpentry", "Formation", nullptr));
        t_bois->setText(QCoreApplication::translate("smartcarpentry", "Gestion du Bois", nullptr));
        search_bois->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher du bois...", nullptr));
        lb_filter_fournisseur->setText(QCoreApplication::translate("smartcarpentry", "Fournisseur :", nullptr));
        combo_filter_fournisseur->setItemText(0, QCoreApplication::translate("smartcarpentry", "Tous", nullptr));
        combo_filter_fournisseur->setItemText(1, QCoreApplication::translate("smartcarpentry", "Bois Tunisie", nullptr));
        combo_filter_fournisseur->setItemText(2, QCoreApplication::translate("smartcarpentry", "For?t Nord", nullptr));
        combo_filter_fournisseur->setItemText(3, QCoreApplication::translate("smartcarpentry", "Import Bois", nullptr));
        combo_filter_fournisseur->setItemText(4, QCoreApplication::translate("smartcarpentry", "Bois Central", nullptr));

        lb_bois_sort->setText(QCoreApplication::translate("smartcarpentry", "Trier par :", nullptr));
        cb_bois_sort_field->setItemText(0, QCoreApplication::translate("smartcarpentry", "Nom", nullptr));
        cb_bois_sort_field->setItemText(1, QCoreApplication::translate("smartcarpentry", "Quantit?", nullptr));
        cb_bois_sort_field->setItemText(2, QCoreApplication::translate("smartcarpentry", "Prix Unitaire", nullptr));
        cb_bois_sort_field->setItemText(3, QCoreApplication::translate("smartcarpentry", "Fournisseur", nullptr));

        cb_bois_sort_order->setItemText(0, QCoreApplication::translate("smartcarpentry", "Croissant", nullptr));
        cb_bois_sort_order->setItemText(1, QCoreApplication::translate("smartcarpentry", "D?croissant", nullptr));

        btn_bois_prev->setText(QCoreApplication::translate("smartcarpentry", "?", nullptr));
        btn_bois_next->setText(QCoreApplication::translate("smartcarpentry", "?", nullptr));
        lb_bois_pagination->setText(QString());
        btn_bois_list_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter un bois", nullptr));
        btn_bois_list_delete->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_bois_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Export PDF", nullptr));
        tabWidget_bois->setTabText(tabWidget_bois->indexOf(tab_bois_list), QCoreApplication::translate("smartcarpentry", "Liste du bois", nullptr));
        lb_bois_stats_title->setText(QCoreApplication::translate("smartcarpentry", "Tableau de Bord ? Analyse du Stock Bois", nullptr));
        btn_bois_stats_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser", nullptr));
        tabWidget_bois->setTabText(tabWidget_bois->indexOf(tab_bois_stats), QCoreApplication::translate("smartcarpentry", "Statistiques", nullptr));
        gb_bois_recommend->setTitle(QCoreApplication::translate("smartcarpentry", "  Moteur de Recommandation IA", nullptr));
        lb_bois_recommend_info->setText(QCoreApplication::translate("smartcarpentry", "Saisissez vos contraintes (budget, usage, r?sistance) et le moteur IA vous recommandera les meilleurs bois disponibles.", nullptr));
        btn_bois_recommend->setText(QCoreApplication::translate("smartcarpentry", "\303\260\305\270\302\244\342\200\223  Lancer la Recommandation", nullptr));
        lb_bois_recommend_result->setText(QCoreApplication::translate("smartcarpentry", "Les recommandations appara?tront ici...", nullptr));
        gb_bois_image_analyse->setTitle(QCoreApplication::translate("smartcarpentry", "  Analyse d'Image IA (D?tection de N?uds)", nullptr));
        lb_bois_image_info->setText(QCoreApplication::translate("smartcarpentry", "Chargez une image d'une pi?ce de bois. Le syst?me analysera les n?uds et attribuera une classe qualit? automatiquement (A/B/C).", nullptr));
        btn_bois_analyze_image->setText(QCoreApplication::translate("smartcarpentry", "\303\260\305\270\342\200\235\302\215  Analyser une Image", nullptr));
        tabWidget_bois->setTabText(tabWidget_bois->indexOf(tab_bois_ai), QCoreApplication::translate("smartcarpentry", "IA & Recommandation", nullptr));
        gf_bois->setTitle(QCoreApplication::translate("smartcarpentry", "  Formulaire bois", nullptr));
        lb1->setText(QCoreApplication::translate("smartcarpentry", "Nom Bois", nullptr));
        bois_nom->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Nom du bois", nullptr));
        lb2->setText(QCoreApplication::translate("smartcarpentry", "Longueur", nullptr));
        lb3->setText(QCoreApplication::translate("smartcarpentry", "Largeur", nullptr));
        lb4->setText(QCoreApplication::translate("smartcarpentry", "Quantite", nullptr));
        lb5->setText(QCoreApplication::translate("smartcarpentry", "Prix Unitaire", nullptr));
        lb6->setText(QCoreApplication::translate("smartcarpentry", "Fournisseur", nullptr));
        btn_bois_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter", nullptr));
        btn_bois_edit->setText(QCoreApplication::translate("smartcarpentry", "Ajouter un bois", nullptr));
        btn_bois_del->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_bois_clr->setText(QCoreApplication::translate("smartcarpentry", "Vider", nullptr));
        tabWidget_bois->setTabText(tabWidget_bois->indexOf(tab_bois_form), QCoreApplication::translate("smartcarpentry", "Ajouter / Modifier du bois", nullptr));
        t_fab->setText(QCoreApplication::translate("smartcarpentry", "Gestion des Fabrications", nullptr));
        search_fab->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher une fabrication...", nullptr));
        lb_filter_etat->setText(QCoreApplication::translate("smartcarpentry", "?tat :", nullptr));
        combo_filter_etat->setItemText(0, QCoreApplication::translate("smartcarpentry", "Tous", nullptr));

        btn_fab_list_modify->setText(QCoreApplication::translate("smartcarpentry", "Ajouter un bois", nullptr));
        btn_fab_list_delete->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_fab_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Export PDF", nullptr));
        tabWidget_fab->setTabText(tabWidget_fab->indexOf(tab_fab_list), QCoreApplication::translate("smartcarpentry", "Liste des fabrications", nullptr));
        gf_fab->setTitle(QCoreApplication::translate("smartcarpentry", "  Formulaire fabrication", nullptr));
        lf0->setText(QCoreApplication::translate("smartcarpentry", "ID Fabrication", nullptr));
        fab_id->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "G?n?r? automatiquement", nullptr));
        lf1->setText(QCoreApplication::translate("smartcarpentry", "Date Debut", nullptr));
        lf2->setText(QCoreApplication::translate("smartcarpentry", "Date Fin Prevue", nullptr));
        lf3->setText(QCoreApplication::translate("smartcarpentry", "Date Fin Reelle", nullptr));
        lf4->setText(QCoreApplication::translate("smartcarpentry", "Nb Personnes", nullptr));
        lf5->setText(QCoreApplication::translate("smartcarpentry", "Etat Fabrication", nullptr));
        lf6->setText(QCoreApplication::translate("smartcarpentry", "Mod?le", nullptr));
        btn_fab_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter", nullptr));
        btn_fab_edit->setText(QCoreApplication::translate("smartcarpentry", "Ajouter un bois", nullptr));
        btn_fab_del->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_fab_clr->setText(QCoreApplication::translate("smartcarpentry", "Vider", nullptr));
        tabWidget_fab->setTabText(tabWidget_fab->indexOf(tab_fab_form), QCoreApplication::translate("smartcarpentry", "Ajouter / Modifier une fabrication", nullptr));
        t_mod->setText(QCoreApplication::translate("smartcarpentry", "Gestion des modeles", nullptr));
        gl_mod->setTitle(QCoreApplication::translate("smartcarpentry", "  Records", nullptr));
        search_mod->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher un modele...", nullptr));
        btn_mod_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Export PDF", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_list), QCoreApplication::translate("smartcarpentry", "Liste des modeles", nullptr));
        gf_mod->setTitle(QCoreApplication::translate("smartcarpentry", "  Formulaire modele  ", nullptr));
        lm0->setText(QCoreApplication::translate("smartcarpentry", "ID Modele", nullptr));
        mod_id->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Auto-generated", nullptr));
        lm1->setText(QCoreApplication::translate("smartcarpentry", "Nom Modele", nullptr));
        mod_nom->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Nom du modele", nullptr));
        lm2->setText(QCoreApplication::translate("smartcarpentry", "Categorie", nullptr));
        lm3->setText(QCoreApplication::translate("smartcarpentry", "Dimensions", nullptr));
        mod_dim->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "ex: 200x100x50", nullptr));
        lm4->setText(QCoreApplication::translate("smartcarpentry", "Type", nullptr));
        lm5->setText(QCoreApplication::translate("smartcarpentry", "Prix Base", nullptr));
        lm6->setText(QCoreApplication::translate("smartcarpentry", "Description", nullptr));
        mod_desc->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Description...", nullptr));
        lm7->setText(QCoreApplication::translate("smartcarpentry", "Cout Estime", nullptr));
        btn_mod_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter", nullptr));
        btn_mod_edit->setText(QCoreApplication::translate("smartcarpentry", "Modifier", nullptr));
        btn_mod_del->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_mod_clr->setText(QCoreApplication::translate("smartcarpentry", "Vider", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_form), QCoreApplication::translate("smartcarpentry", "Ajouter / Modifier", nullptr));
        gb_mod_stats_total->setTitle(QCoreApplication::translate("smartcarpentry", "  Vue globale  ", nullptr));
        lb_mod_stats_total_title->setText(QCoreApplication::translate("smartcarpentry", "Total modeles", nullptr));
        lb_mod_stats_total->setText(QCoreApplication::translate("smartcarpentry", "0", nullptr));
        lb_mod_stats_avg_price_title->setText(QCoreApplication::translate("smartcarpentry", "Prix moyen", nullptr));
        lb_mod_stats_avg_price->setText(QCoreApplication::translate("smartcarpentry", "0.00", nullptr));
        gb_mod_stats_price->setTitle(QCoreApplication::translate("smartcarpentry", "  Prix  ", nullptr));
        lb_mod_stats_min_price_title->setText(QCoreApplication::translate("smartcarpentry", "Prix min", nullptr));
        lb_mod_stats_min_price->setText(QCoreApplication::translate("smartcarpentry", "0.00", nullptr));
        lb_mod_stats_max_price_title->setText(QCoreApplication::translate("smartcarpentry", "Prix max", nullptr));
        lb_mod_stats_max_price->setText(QCoreApplication::translate("smartcarpentry", "0.00", nullptr));
        gb_mod_stats_cost->setTitle(QCoreApplication::translate("smartcarpentry", "  Cout  ", nullptr));
        lb_mod_stats_avg_cost_title->setText(QCoreApplication::translate("smartcarpentry", "Cout moyen", nullptr));
        lb_mod_stats_avg_cost->setText(QCoreApplication::translate("smartcarpentry", "0.00", nullptr));
        gb_mod_stats_top->setTitle(QCoreApplication::translate("smartcarpentry", "  Top modeles (prix + marge)  ", nullptr));
        gb_mod_stats_cat->setTitle(QCoreApplication::translate("smartcarpentry", "  Par categorie  ", nullptr));
        gb_mod_stats_chart_cat->setTitle(QCoreApplication::translate("smartcarpentry", "  Repartition par categorie  ", nullptr));
        gb_mod_stats_chart_price->setTitle(QCoreApplication::translate("smartcarpentry", "  Prix vs Cout moyen vs Marge  ", nullptr));
        btn_mod_stats_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser les statistiques", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_stats), QCoreApplication::translate("smartcarpentry", "Statistiques", nullptr));
        gb_mod_estimation->setTitle(QCoreApplication::translate("smartcarpentry", "  Estimation par image  ", nullptr));
        lb_mod_estimate_hint->setText(QCoreApplication::translate("smartcarpentry", "Importer une image pour estimer le cout et le temps de realisation.", nullptr));
        btn_mod_estimate_image->setText(QCoreApplication::translate("smartcarpentry", "Lancer l'estimation", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_estimation), QCoreApplication::translate("smartcarpentry", "Estimation", nullptr));
        gb_mod_budget->setTitle(QCoreApplication::translate("smartcarpentry", "  Generation par budget  ", nullptr));
        lb_mod_budget_label->setText(QCoreApplication::translate("smartcarpentry", "Budget", nullptr));
        btn_mod_budget_generate->setText(QCoreApplication::translate("smartcarpentry", "Generer une suggestion", nullptr));
        lb_mod_budget_result->setText(QCoreApplication::translate("smartcarpentry", "Aucune suggestion", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_budget), QCoreApplication::translate("smartcarpentry", "Budget", nullptr));
        gb_mod_perf_entry->setTitle(QCoreApplication::translate("smartcarpentry", "  Saisie \342\200\224 estimation cout & performance  ", nullptr));
        lb_mod_perf_type->setText(QCoreApplication::translate("smartcarpentry", "Type de modele", nullptr));
        lb_mod_perf_dimensions->setText(QCoreApplication::translate("smartcarpentry", "Dimensions (cm)", nullptr));
        le_mod_perf_dimensions->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Ex. 140 x 80 x 75", nullptr));
        lb_mod_perf_materiau->setText(QCoreApplication::translate("smartcarpentry", "Materiau", nullptr));
        lb_mod_perf_complexite->setText(QCoreApplication::translate("smartcarpentry", "Complexite", nullptr));
        lb_mod_perf_quantite->setText(QCoreApplication::translate("smartcarpentry", "Quantite", nullptr));
        btn_mod_perf_calculate->setText(QCoreApplication::translate("smartcarpentry", "Calculer", nullptr));
        gb_mod_perf_results->setTitle(QCoreApplication::translate("smartcarpentry", "  Resultats  ", nullptr));
        tb_mod_perf_result->setHtml(QCoreApplication::translate("smartcarpentry", "<p style='color:#94a3b8;'>Remplissez le formulaire puis cliquez sur <b>Calculer</b>.</p>", nullptr));
        gb_mod_perf_chart->setTitle(QCoreApplication::translate("smartcarpentry", "  Projection cout (2026\342\200\2232030)  ", nullptr));
        btn_mod_perf_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Exporter en PDF", nullptr));
        tabWidget_mod->setTabText(tabWidget_mod->indexOf(tab_mod_performance), QCoreApplication::translate("smartcarpentry", "Cout & performance", nullptr));
        t_pc->setText(QCoreApplication::translate("smartcarpentry", "Gestion des Pieces", nullptr));
        search_pc->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Rechercher une piece...", nullptr));
        lb_filter_matiere->setText(QCoreApplication::translate("smartcarpentry", "Matiere :", nullptr));
        combo_filter_matiere->setItemText(0, QCoreApplication::translate("smartcarpentry", "Toutes", nullptr));
        combo_filter_matiere->setItemText(1, QCoreApplication::translate("smartcarpentry", "Bois massif", nullptr));
        combo_filter_matiere->setItemText(2, QCoreApplication::translate("smartcarpentry", "MDF", nullptr));
        combo_filter_matiere->setItemText(3, QCoreApplication::translate("smartcarpentry", "Contreplaque", nullptr));
        combo_filter_matiere->setItemText(4, QCoreApplication::translate("smartcarpentry", "Aluminium", nullptr));
        combo_filter_matiere->setItemText(5, QCoreApplication::translate("smartcarpentry", "PVC", nullptr));

        lb_pc_sort_by->setText(QCoreApplication::translate("smartcarpentry", "Trier par :", nullptr));
        pc_sort_by->setItemText(0, QCoreApplication::translate("smartcarpentry", "ID", nullptr));
        pc_sort_by->setItemText(1, QCoreApplication::translate("smartcarpentry", "Nom", nullptr));
        pc_sort_by->setItemText(2, QCoreApplication::translate("smartcarpentry", "Code", nullptr));
        pc_sort_by->setItemText(3, QCoreApplication::translate("smartcarpentry", "Quantite", nullptr));
        pc_sort_by->setItemText(4, QCoreApplication::translate("smartcarpentry", "Matiere", nullptr));
        pc_sort_by->setItemText(5, QCoreApplication::translate("smartcarpentry", "Longueur", nullptr));
        pc_sort_by->setItemText(6, QCoreApplication::translate("smartcarpentry", "Largeur", nullptr));
        pc_sort_by->setItemText(7, QCoreApplication::translate("smartcarpentry", "Epaisseur", nullptr));

        pc_sort_order->setItemText(0, QCoreApplication::translate("smartcarpentry", "Decroissant", nullptr));
        pc_sort_order->setItemText(1, QCoreApplication::translate("smartcarpentry", "Croissant", nullptr));

        btn_pc_list_modify->setText(QCoreApplication::translate("smartcarpentry", "Ajouter une piece", nullptr));
        btn_pc_list_delete->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_pc_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Export PDF", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_list), QCoreApplication::translate("smartcarpentry", "Liste des pieces", nullptr));
        gf_pc->setTitle(QCoreApplication::translate("smartcarpentry", "  Formulaire piece", nullptr));
        lp0->setText(QCoreApplication::translate("smartcarpentry", "ID Piece", nullptr));
        pc_id->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Auto-generated", nullptr));
        lp1->setText(QCoreApplication::translate("smartcarpentry", "Nom Piece", nullptr));
        pc_nom->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Nom de la piece", nullptr));
        lp2->setText(QCoreApplication::translate("smartcarpentry", "Code Piece", nullptr));
        pc_code->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Code unique", nullptr));
        lp3->setText(QCoreApplication::translate("smartcarpentry", "Quantite", nullptr));
        lp4->setText(QCoreApplication::translate("smartcarpentry", "Bois / Matiere", nullptr));
        lp5->setText(QCoreApplication::translate("smartcarpentry", "Longueur (mm)", nullptr));
        lp6->setText(QCoreApplication::translate("smartcarpentry", "Largeur (mm)", nullptr));
        lp7->setText(QCoreApplication::translate("smartcarpentry", "Epaisseur (mm)", nullptr));
        lp8->setText(QCoreApplication::translate("smartcarpentry", "Modele", nullptr));
        lp9->setText(QCoreApplication::translate("smartcarpentry", "ID Modele", nullptr));
        lp10->setText(QCoreApplication::translate("smartcarpentry", "ID Bois", nullptr));
        btn_pc_add->setText(QCoreApplication::translate("smartcarpentry", "Ajouter", nullptr));
        btn_pc_edit->setText(QCoreApplication::translate("smartcarpentry", "Modifier", nullptr));
        btn_pc_del->setText(QCoreApplication::translate("smartcarpentry", "Supprimer", nullptr));
        btn_pc_clr->setText(QCoreApplication::translate("smartcarpentry", "Vider", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_form), QCoreApplication::translate("smartcarpentry", "Ajouter / Modifier une piece", nullptr));
        gb_pc_stats_total->setTitle(QCoreApplication::translate("smartcarpentry", "  Vue globale", nullptr));
        lb_pc_stats_total_title->setText(QCoreApplication::translate("smartcarpentry", "Pieces generees", nullptr));
        lb_pc_stats_total_value->setText(QCoreApplication::translate("smartcarpentry", "128", nullptr));
        lb_pc_stats_total_hint->setText(QCoreApplication::translate("smartcarpentry", "Cette semaine", nullptr));
        gb_pc_stats_yield->setTitle(QCoreApplication::translate("smartcarpentry", "  Rendement", nullptr));
        lb_pc_stats_yield_title->setText(QCoreApplication::translate("smartcarpentry", "Taux d'utilisation", nullptr));
        lb_pc_stats_yield_value->setText(QCoreApplication::translate("smartcarpentry", "84%", nullptr));
        gb_pc_stats_loss->setTitle(QCoreApplication::translate("smartcarpentry", "  Perte matiere", nullptr));
        lb_pc_stats_loss_title->setText(QCoreApplication::translate("smartcarpentry", "Chute moyenne", nullptr));
        lb_pc_stats_loss_value->setText(QCoreApplication::translate("smartcarpentry", "16%", nullptr));
        lb_pc_stats_loss_hint->setText(QCoreApplication::translate("smartcarpentry", "Objectif < 12%", nullptr));
        gb_pc_stats_materials->setTitle(QCoreApplication::translate("smartcarpentry", "  Repartition par matiere", nullptr));
        lb_pc_stats_sort->setText(QCoreApplication::translate("smartcarpentry", "Tri :", nullptr));
        pc_stats_sort_by->setItemText(0, QCoreApplication::translate("smartcarpentry", "Quantite totale", nullptr));
        pc_stats_sort_by->setItemText(1, QCoreApplication::translate("smartcarpentry", "Nombre de references", nullptr));
        pc_stats_sort_by->setItemText(2, QCoreApplication::translate("smartcarpentry", "Surface totale", nullptr));

        pc_stats_sort_order->setItemText(0, QCoreApplication::translate("smartcarpentry", "Decroissant", nullptr));
        pc_stats_sort_order->setItemText(1, QCoreApplication::translate("smartcarpentry", "Croissant", nullptr));

        lb_pc_stats_mat_1->setText(QCoreApplication::translate("smartcarpentry", "Chene massif", nullptr));
        lb_pc_stats_mat_2->setText(QCoreApplication::translate("smartcarpentry", "MDF", nullptr));
        lb_pc_stats_mat_3->setText(QCoreApplication::translate("smartcarpentry", "Contreplaque", nullptr));
        gb_pc_stats_pipeline->setTitle(QCoreApplication::translate("smartcarpentry", "  Pipeline de production", nullptr));
        lb_pc_stats_pipeline_title->setText(QCoreApplication::translate("smartcarpentry", "Charge et flux en direct", nullptr));
        txt_pc_stats_pipeline->setHtml(QCoreApplication::translate("smartcarpentry", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI','sans-serif'; font-size:14px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ffffff;\">Ligne A</span> : 24 pieces en attente</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ffffff;\">Ligne B</span> : 12 pieces en finition</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-r"
                        "ight:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ffffff;\">Mode auto IA</span> : 6 plans proposes aujourd'hui</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ffffff;\">Economies estimees</span> : 18.4% de matiere conservee</p></body></html>", nullptr));
        btn_pc_stats_refresh->setText(QCoreApplication::translate("smartcarpentry", "Actualiser les statistiques", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_stats), QCoreApplication::translate("smartcarpentry", "Statistiques", nullptr));
        gb_pc_model_source->setTitle(QCoreApplication::translate("smartcarpentry", "  Image du modele source", nullptr));
        lb_pc_model_dropzone->setText(QCoreApplication::translate("smartcarpentry", "Deposer l'image du modele ici\n"
"\n"
"PNG / JPG / rendu 3D / plan de facade", nullptr));
        btn_pc_model_import->setText(QCoreApplication::translate("smartcarpentry", "Importer une image", nullptr));
        btn_pc_model_preview->setText(QCoreApplication::translate("smartcarpentry", "Previsualiser", nullptr));
        gb_pc_model_settings->setTitle(QCoreApplication::translate("smartcarpentry", "  Parametres de segmentation", nullptr));
        lb_pc_model_selected->setText(QCoreApplication::translate("smartcarpentry", "Modele cible", nullptr));
        lb_pc_model_precision->setText(QCoreApplication::translate("smartcarpentry", "Precision IA", nullptr));
        pc_ai_precision->setItemText(0, QCoreApplication::translate("smartcarpentry", "Rapide", nullptr));
        pc_ai_precision->setItemText(1, QCoreApplication::translate("smartcarpentry", "Equilibree", nullptr));
        pc_ai_precision->setItemText(2, QCoreApplication::translate("smartcarpentry", "Maximale", nullptr));

        lb_pc_model_tolerance->setText(QCoreApplication::translate("smartcarpentry", "Tolerance de coupe (mm)", nullptr));
        chk_pc_ai_edges->setText(QCoreApplication::translate("smartcarpentry", "Detecter les contours", nullptr));
        chk_pc_ai_merge->setText(QCoreApplication::translate("smartcarpentry", "Fusion des zones symetriques", nullptr));
        txt_pc_manual_notes->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Definir ici les zones, annotations et contraintes manuelles de decoupage...", nullptr));
        btn_pc_manual_generate->setText(QCoreApplication::translate("smartcarpentry", "Generer les pieces manuellement", nullptr));
        tabWidget_pc_generation_modes->setTabText(tabWidget_pc_generation_modes->indexOf(tab_pc_generation_manual), QCoreApplication::translate("smartcarpentry", "Manuel", nullptr));
        lb_pc_auto_hint->setText(QCoreApplication::translate("smartcarpentry", "L'IA analyse l'image du modele, detecte les contours utiles, propose un decoupage intelligent et prepare automatiquement les pieces exploitables.", nullptr));
        btn_pc_auto_generate->setText(QCoreApplication::translate("smartcarpentry", "Lancer l'analyse automatique", nullptr));
        tabWidget_pc_generation_modes->setTabText(tabWidget_pc_generation_modes->indexOf(tab_pc_generation_auto), QCoreApplication::translate("smartcarpentry", "Automatique IA", nullptr));
        tabWidget_pc_model_pages->setTabText(tabWidget_pc_model_pages->indexOf(tab_pc_model_prepare), QCoreApplication::translate("smartcarpentry", "Preparation", nullptr));
        gb_pc_model_result->setTitle(QCoreApplication::translate("smartcarpentry", "  Resultat du decoupage modele -> piece", nullptr));
        lb_pc_model_result_summary->setText(QCoreApplication::translate("smartcarpentry", "Sortie attendue: contours detectes, nombre de pieces proposees, zones de coupe critiques, estimation de perte et validation avant insertion en base.", nullptr));
        txt_pc_model_result->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Les details des pieces detectees et le journal de segmentation apparaissent ici...", nullptr));
        tabWidget_pc_model_pages->setTabText(tabWidget_pc_model_pages->indexOf(tab_pc_model_draft), QCoreApplication::translate("smartcarpentry", "Brouillon et validation", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_model_ai), QCoreApplication::translate("smartcarpentry", "Modele -> Piece", nullptr));
        gb_pc_nesting_sheet->setTitle(QCoreApplication::translate("smartcarpentry", "  Parametres de plaque", nullptr));
        chk_pc_nesting_save_scrap->setText(QCoreApplication::translate("smartcarpentry", "Recycler les chutes utiles", nullptr));
        lb_pc_nesting_width->setText(QCoreApplication::translate("smartcarpentry", "Largeur plaque (mm)", nullptr));
        lb_pc_nesting_length->setText(QCoreApplication::translate("smartcarpentry", "Longueur plaque (mm)", nullptr));
        pc_nesting_algo->setItemText(0, QCoreApplication::translate("smartcarpentry", "Guillotine rapide", nullptr));
        pc_nesting_algo->setItemText(1, QCoreApplication::translate("smartcarpentry", "Best-fit intelligent", nullptr));
        pc_nesting_algo->setItemText(2, QCoreApplication::translate("smartcarpentry", "Conservation maximale", nullptr));

        chk_pc_nesting_rotate->setText(QCoreApplication::translate("smartcarpentry", "Autoriser rotation 90 degres", nullptr));
        lb_pc_nesting_algo->setText(QCoreApplication::translate("smartcarpentry", "Strategie", nullptr));
        lb_pc_nesting_gap->setText(QCoreApplication::translate("smartcarpentry", "Espacement de securite (mm)", nullptr));
        gb_pc_nesting_metrics->setTitle(QCoreApplication::translate("smartcarpentry", "  Qualite du plan", nullptr));
        lb_pc_nesting_eff->setText(QCoreApplication::translate("smartcarpentry", "Utilisation matiere", nullptr));
        lb_pc_nesting_scrap->setText(QCoreApplication::translate("smartcarpentry", "Chute recuperable", nullptr));
        lb_pc_nesting_summary->setText(QCoreApplication::translate("smartcarpentry", "Plan optimise avec priorite sur la conservation de matiere et la reduction des coupes longues.", nullptr));
        btn_pc_nesting_run->setText(QCoreApplication::translate("smartcarpentry", "Lancer l'optimisation", nullptr));
        gb_pc_nesting_canvas->setTitle(QCoreApplication::translate("smartcarpentry", "  Plan de decoupe intelligent", nullptr));
        lb_pc_nesting_canvas->setText(QCoreApplication::translate("smartcarpentry", "Apercu du placement optimal des pieces sur la plaque\n"
"\n"
"Zones compactes | coupes reduites | chutes reutilisables", nullptr));
        gb_pc_nesting_actions->setTitle(QCoreApplication::translate("smartcarpentry", "  Actions et sortie", nullptr));
        btn_pc_nesting_export_pdf->setText(QCoreApplication::translate("smartcarpentry", "Exporter le plan PDF", nullptr));
        btn_pc_nesting_export_csv->setText(QCoreApplication::translate("smartcarpentry", "Exporter la liste des coupes", nullptr));
        txt_pc_nesting_log->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Journal d'optimisation, suggestions IA et sequence des coupes...", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_nesting), QCoreApplication::translate("smartcarpentry", "Decoupage intelligent", nullptr));
        gb_pc_manual_source->setTitle(QCoreApplication::translate("smartcarpentry", "  Source et contraintes", nullptr));
        lb_pc_manual_modele->setText(QCoreApplication::translate("smartcarpentry", "Modele", nullptr));
        lb_pc_manual_bois->setText(QCoreApplication::translate("smartcarpentry", "Bois / plaque", nullptr));
        lb_pc_manual_passes->setText(QCoreApplication::translate("smartcarpentry", "Nb passes de coupe", nullptr));
        chk_pc_manual_keep_offcuts->setText(QCoreApplication::translate("smartcarpentry", "Conserver les chutes utiles", nullptr));
        chk_pc_manual_axis_lock->setText(QCoreApplication::translate("smartcarpentry", "Verrouiller les axes", nullptr));
        gb_pc_manual_canvas->setTitle(QCoreApplication::translate("smartcarpentry", "  Zone de tracage manuel", nullptr));
        lb_pc_manual_canvas->setText(QCoreApplication::translate("smartcarpentry", "Surface de decoupage manuel\n"
"\n"
"Tracer les lignes, definir les segments et valider l'ordre des coupes", nullptr));
        gb_pc_manual_plan->setTitle(QCoreApplication::translate("smartcarpentry", "  Plan manuel et instructions", nullptr));
        txt_pc_manual_cut_plan->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Exemple :\n"
"1. Couper bande verticale 450 mm\n"
"2. Rotation de la plaque\n"
"3. Coupe horizontale 820 mm\n"
"4. Garder la chute 300x120 en stock", nullptr));
        btn_pc_manual_add_step->setText(QCoreApplication::translate("smartcarpentry", "Ajouter une etape", nullptr));
        btn_pc_manual_validate->setText(QCoreApplication::translate("smartcarpentry", "Valider le plan manuel", nullptr));
        btn_pc_manual_export->setText(QCoreApplication::translate("smartcarpentry", "Exporter les instructions", nullptr));
        tabWidget_pc->setTabText(tabWidget_pc->indexOf(tab_pc_manual_cut), QCoreApplication::translate("smartcarpentry", "Decoupage manuel", nullptr));
        t_set->setText(QCoreApplication::translate("smartcarpentry", "Param?tres", nullptr));
        gf_set->setTitle(QCoreApplication::translate("smartcarpentry", "  Connexion ? la base de donn?es", nullptr));
        ls0->setText(QCoreApplication::translate("smartcarpentry", "H?te", nullptr));
        set_host->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "localhost", nullptr));
        ls1->setText(QCoreApplication::translate("smartcarpentry", "Port", nullptr));
        set_port->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "1521", nullptr));
        ls2->setText(QCoreApplication::translate("smartcarpentry", "Base de donn?es", nullptr));
        set_db->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "XE", nullptr));
        ls3->setText(QCoreApplication::translate("smartcarpentry", "Nom d'utilisateur", nullptr));
        set_user->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "system", nullptr));
        ls4->setText(QCoreApplication::translate("smartcarpentry", "Mot de passe", nullptr));
        set_pass->setPlaceholderText(QCoreApplication::translate("smartcarpentry", "Mot de passe", nullptr));
        btn_test_conn->setText(QCoreApplication::translate("smartcarpentry", "Tester la connexion", nullptr));
        btn_save_settings->setText(QCoreApplication::translate("smartcarpentry", "Enregistrer les param?tres", nullptr));
        gf_app->setTitle(QCoreApplication::translate("smartcarpentry", "  Application", nullptr));
        ttsenable->setText(QCoreApplication::translate("smartcarpentry", "Enable Text-to-Speech", nullptr));
        qrenable->setText(QCoreApplication::translate("smartcarpentry", "Enable QR Code", nullptr));
    } // retranslateUi

};

namespace Ui {
    class smartcarpentry: public Ui_smartcarpentry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMARTCARPENTRY_H
