from datetime import date
from odoo import models, fields, api
from odoo.tools import email_re
from odoo.exceptions import ValidationError


class HmsPatientLogs(models.Model):
    _name = 'hms.patient.logs'

    patient_id = fields.Many2one(comodel_name='hms.patient')
    description = fields.Text()


class HmsPatient(models.Model):
    _name = 'hms.patient'
    _rec_name = 'first_name'

    first_name = fields.Char(required=True)
    last_name = fields.Char(required=True)
    birth_date = fields.Date()
    history = fields.Html()
    cr_ratio = fields.Float()
    blood_type = fields.Selection([
        ('A', 'A'),
        ('B', 'B'),
        ('O', 'O'),
        ('AB', 'AB')
    ])
    state = fields.Selection([
        ('undetermined', 'Undetermined'),
        ('good', 'Good'),
        ('fair', 'Fair'),
        ('serious', 'Serious')
    ], default='undetermined')
    pcr = fields.Boolean()
    image = fields.Image()
    address = fields.Text()
    age = fields.Integer(compute = '_compute_age', store=True)
    department_id = fields.Many2one(comodel_name='hms.department')
    department_capacity = fields.Integer(related='department_id.capacity', readonly=True)
    doctors_ids = fields.Many2many(comodel_name='hms.doctor')
    logs = fields.One2many(comodel_name='hms.patient.logs' , inverse_name='patient_id', string='State Logs')
    email = fields.Char()
    
    _sql_constraints = [
        ('email_unique', 'UNIQUE(email)', 'Email must be unique')
    ]

    @api.constrains('email')
    def validate_email(self):
        for record in self:
            if record.email and not email_re.match(record.email):
                raise models.ValidationError('Invalid email address')
            
    @api.onchange('state')
    def track_state(self):
        for record in self:
            if record._origin and record._origin.state != record.state:
                vals = {
                    'description' : f'State changed to {record.state}',
                    'patient_id' : record._origin.id
                }
                record.env['hms.patient.logs'].create(vals)

    @api.depends('birth_date')
    def _compute_age(self):
        for rec in self:
            if rec.birth_date:
                today = date.today()
                rec.age = today.year - rec.birth_date.year - (
                        (today.month, today.day) < (rec.birth_date.month, rec.birth_date.day))
            else:
                rec.age = 0

    @api.onchange('age')
    def track_age(self):
        if self.age and self.age < 30 and not self.pcr:
            self.pcr = True
            return {
                'warning': {
                    'title': 'PCR is auto checked',
                    'message': 'Age is under 30 and PCR is checked automatically'
                }
            }
        
    @api.onchange('pcr')
    def track_pcr(self):
        if not self.pcr and self.age and self.age < 30:
            self.pcr = True
            return {
                'warning': {
                    'title': 'PCR is auto checked',
                    'message': 'Age is under 30 and PCR is checked automatically'
                }
            }