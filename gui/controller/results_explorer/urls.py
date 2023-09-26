# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2023 Open Risk (https://www.openriskmanagement.com)+
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

app_name = 'results_explorer'

urlpatterns = [
    re_path(r'^$', views.ResultsList.as_view(), name='results_list'),
    re_path(r'^results_list$', views.ResultsList.as_view(), name='results_list'),
    re_path(r'^result_group_list$', views.ResultGroupList.as_view(), name='result_group_list'),
    re_path(r'^results_view/(?P<pk>\d+)$', views.results_view, name='results_view'),
    re_path(r'^result_group_view/(?P<pk>\d+)$', views.result_group_view, name='result_group_view'),
    re_path(r'^result_group_logfiles/(?P<pk>\d+)$', views.result_group_logfiles, name='result_group_logfiles'),
]
