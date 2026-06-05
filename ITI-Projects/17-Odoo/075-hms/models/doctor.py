from odoo import models, fields

class HmsDoctor(models.Model):
  _name = 'hms.doctor'
  _rec_name = 'first_name'

  first_name = fields.Char(required=True)
  last_name = fields.Char(required=True)
  image = fields.Image()
  patients = fields.Many2many(comodel_name='hms.patient')