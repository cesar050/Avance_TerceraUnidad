from django.contrib import admin
from .models import ServoData

@admin.register(ServoData)
class ServoDataAdmin(admin.ModelAdmin):
    list_display = ('esp32_ip', 'timestamp', 'base', 'hombro', 'muneca', 'codo', 'pinza', 'camera', 'forward', 'backward', 'left', 'right')
    readonly_fields = ('timestamp',)


