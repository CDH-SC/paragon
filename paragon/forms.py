from django import forms
from django.contrib.auth.forms import UserChangeForm, ReadOnlyPasswordHashField
from django.contrib.auth.models import User

class UpdateForm(forms.ModelForm):
    #password = ReadOnlyPasswordHashField(label="password",
    #                                     help_text="""Raw passwords are not stored, so there is no way to see this
    #                                     user's password, but you can change the password using <a href=\"password/\">
    #                                     this form</a>""")
    class Meta:
        model = User
        fields = ('password', 'email')
        #widgets = {
        #    'password': forms.CharField(),
        #}
    def clean_password(self):
        # Regardless of what the user provides, return the initial value.
        # This is done here, rather than on the field, because the field does
        # not have access to the initial value
        return self.initial["password"]
