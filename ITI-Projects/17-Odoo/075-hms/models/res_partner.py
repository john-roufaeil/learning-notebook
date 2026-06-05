from odoo import models, fields, api
from odoo.exceptions import ValidationError

class ResPartner(models.Model):
  _inherit = 'res.partner'

  related_patient_id = fields.Many2one('hms.patient', string='Related Patient')


  @api.constrains('related_patient_id')
  def check_patient_unique_email_accross_customers(self):
    for record in self:
      if record.related_patient_id and record.related_patient_id.email:
        existing_partner = self.env['res.partner'].search([
          ('related_patient_id.email', '=', record.related_patient_id.email),
          ('id', '!=', record.id)
        ], limit=1)
        if existing_partner:
          raise ValidationError('The related patient email must be unique across customers.')