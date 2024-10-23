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
from django.urls import path, include, reverse_lazy
from django.urls import re_path
from django.views.generic import RedirectView

from . import settings

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', RedirectView.as_view(url=reverse_lazy('admin:index'))),
    re_path(r'^results_explorer/', include('results_explorer.urls')),
    re_path(r'^workflow_explorer/', include('workflow_explorer.urls')),
    re_path(r'^visualization_explorer/', include('visualization_explorer.urls')),
]

if settings.DEBUG:
    import debug_toolbar

    urlpatterns = [
                      path('__debug__/', include(debug_toolbar.urls)),
                  ] + urlpatterns
