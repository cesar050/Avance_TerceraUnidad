from django.core.exceptions import ValidationError
from django.utils.translation import gettext_lazy as _

class ContrasenaValidator:
    def validate(self, password, user=None):
        if len(password) < 8:
            raise ValidationError(
                _("La contraseña debe tener al menos 8 caracteres."),
                code='password_too_short',
            )
        if password.isdigit():
            raise ValidationError(
                _("La contraseña no puede ser completamente numérica."),
                code='password_entirely_numeric',
            )
        if password.isalpha():
            raise ValidationError(
                _("La contraseña no puede ser completamente alfabética."),
                code='password_entirely_alpha',
            )
    def get_help_text(self):
        return _("La contraseña debe tener al menos 8 caracteres y no puede ser completamente numérica."),


