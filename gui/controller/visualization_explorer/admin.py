# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2025 Open Risk (https://www.openriskmanagement.com)+
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
from django.http import HttpResponseRedirect
from django.urls import reverse
from django.utils.safestring import mark_safe
from django.utils.translation import gettext as _
from jsoneditor.forms import JSONEditor
from prettyjson import PrettyJSONWidget

from visualization_explorer.models import Visualization, VegaSpecification, VegaLiteSpecification


class VisualizationAdmin(admin.ModelAdmin):
    formfield_overrides = {
        JSONField: {'widget': PrettyJSONWidget(attrs={'initiaĺ': 'parsed'})},
    }
    #
    # Searchable fields
    #
    search_fields = ['description']
    list_display = ('name', 'view_link', 'objective', 'description', 'last_change_date',)
    list_filter = ('objective',)
    save_as = True
    view_on_site = True

    def view_link(self, obj):
        return mark_safe('<a href="{0}">{1}</a>'.format(obj.get_absolute_url(), _("View on site")))

    view_link.allow_tags = True
    view_link.short_description = _("View on site")

    def response_change(self, request, obj, post_url_continue=None):
        """This makes the response after adding go to another apps changelist for some model"""
        return HttpResponseRedirect(reverse("visualization_explorer:Visualization_list"))


class VegaSpecificationAdmin(admin.ModelAdmin):
    formfield_overrides = {
        JSONField: {'widget': JSONEditor},
    }
    #
    # Searchable fields
    #
    search_fields = ['description']
    list_display = ('title', 'width', 'height', 'description',)
    # list_filter = ('objective',)
    save_as = True
    view_on_site = False


class VegaLiteSpecificationAdmin(admin.ModelAdmin):
    formfield_overrides = {
        JSONField: {'widget': JSONEditor},
    }

    search_fields = ['description']
    list_display = ('title', 'description',)
    save_as = True
    view_on_site = False


admin.site.register(Visualization, VisualizationAdmin)
admin.site.register(VegaSpecification, VegaSpecificationAdmin)
admin.site.register(VegaLiteSpecification, VegaLiteSpecificationAdmin)
