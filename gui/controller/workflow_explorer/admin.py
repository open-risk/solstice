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

from workflow_explorer.models import Workflow, WorkflowJSON


class WorkflowJSONAdmin(admin.ModelAdmin):
    formfield_overrides = {
        # JSONField: {'widget': JSONEditor},
        JSONField: {'widget': PrettyJSONWidget(attrs={'initial': 'parsed'})}
    }

    list_display = ('name',)

    save_as = True
    view_on_site = True


class WorkflowAdmin(admin.ModelAdmin):
    formfield_overrides = {
        # JSONField: {'widget': JSONEditor},
        JSONField: {'widget': PrettyJSONWidget(attrs={'initial': 'parsed'})}
    }
    #
    # Searchable fields
    #
    search_fields = ['workflow_description']
    list_display = ('workflow_id', 'name', 'objective', 'workflow_description', 'last_change_date',)
    list_filter = ('objective', 'workflow_type', 'workflow_status', 'portfolio_data_mode',
                   'scenario_data_mode', 'single_asset_flag')
    save_as = True

    view_on_site = False


admin.site.register(Workflow, WorkflowAdmin)
admin.site.register(WorkflowJSON, WorkflowJSONAdmin)
