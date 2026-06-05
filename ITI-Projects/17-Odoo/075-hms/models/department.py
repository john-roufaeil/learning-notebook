from odoo import models, fields, api

class HmsDepartment(models.Model):
  _name = 'hms.department'

  name = fields.Char(required=True)
  capacity = fields.Integer()
  is_opened = fields.Boolean(default=True)
  patients = fields.One2many(comodel_name='hms.patient', inverse_name='department_id')