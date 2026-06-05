{
    'name': 'Hospital Management System',
    'depends': ['sale'],
    'data': [
      'security/res_groups.xml',
      'security/ir.model.access.csv',
      'views/hms_patient_views.xml',
      'views/hms_department_views.xml',
      'views/hms_doctor_views.xml',
      'views/res_partner_views.xml', 
      'reports/reports.xml',
      'reports/hms_patient_report_temp.xml'
    ],
    'installable': True,
    'application': True,
}