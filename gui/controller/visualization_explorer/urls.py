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

from django.urls import re_path
from . import views

app_name = 'visualization_explorer'

urlpatterns = [
    re_path(r'^$', views.VisualizationList.as_view(), name='Visualization_list'),
    re_path(r'^visualization_view/(?P<pk>\d+)$', views.Visualization_view, name='Visualization_view'),
    re_path(r'^objectives$', views.VisualizationObjectiveList.as_view(), name='Visualization_objective_list'),
    re_path(r'^visualization_interactive/(?P<pk>\d+)$', views.Visualization_interactive, name='Visualization_interactive'),
]