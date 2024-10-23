# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from django.contrib import admin
from django.db.models import JSONField
from prettyjson import PrettyJSONWidget

from results_explorer.models import Calculation, ResultGroup


class CalculationAdmin(admin.ModelAdmin):
    fields = ('workflow', 'workflow_data', 'results_data', 'logfile', 'result_group')
    search_fields = ['results_data', 'workflow_data']
    list_display = ('workflow', 'result_group', 'creation_date')
    date_hierarchy = ('creation_date')

    formfield_overrides = {
        JSONField: {'widget': PrettyJSONWidget(attrs={'initiaĺ': 'parsed'})},
    }
    save_as = False
    view_on_site = True


class ResultGroupAdmin(admin.ModelAdmin):
    fields = ('group_type',)
    list_filter = ('group_type',)
    list_display = ('group_type', 'creation_date')
    date_hierarchy = ('creation_date')

    save_as = False
    view_on_site = True


admin.site.register(Calculation, CalculationAdmin)
admin.site.register(ResultGroup, ResultGroupAdmin)
