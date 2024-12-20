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


from django.db import models
from django.db.models import JSONField
from django.urls import reverse
from django.utils.timezone import now

from workflow_explorer.models import Workflow


class ResultGroup(models.Model):
    """
    Data object holds a group of calculation results
    """
    creation_date = models.DateTimeField(auto_now_add=True)
    group_type = models.IntegerField(default=0)

    # The number of results include in the group
    # Must be manually augmented whenever there is a result added or deleted
    # mirrors self.runs in Controller/Results.py

    calculation_count = models.IntegerField(default=0)

    # the playbook that created this result group (if available)
    # ATTN result groups can also be formed in ad-hoc ways (e.g. user defined collections)
    # in that case there is no playbook associated and thus standardardized reports
    # and visualization are not available

    # TODO Does not make strict sense for a collection
    calculation_timestamp = models.DateTimeField(default=now)

    def __str__(self):
        return str(self.pk)

    def get_absolute_url(self):
        return reverse('admin:results_explorer_result_group_change', kwargs={'pk': self.pk})

    class Meta:
        verbose_name = "Result Group"
        verbose_name_plural = "Result Groups"


class Calculation(models.Model):
    result_group = models.ForeignKey(ResultGroup, on_delete=models.CASCADE, null=True, blank=True,
                                     help_text="Result Group to which this Calculation belong (if any)")

    workflow = models.ForeignKey(Workflow, on_delete=models.CASCADE, default=1)

    workflow_data = JSONField(null=True, blank=True, help_text="Verbatim storage of the calculation input "
                                                               "in JSON format")

    creation_date = models.DateTimeField(auto_now_add=True)

    logfile = models.TextField(null=True, blank=True, help_text="Verbatim storage of the calculation logfile")
    results_data = JSONField(null=True, blank=True, help_text="Verbatim storage of the calculation results "
                                                              "in JSON format")
    calculation_timestamp = models.DateTimeField(default=now)

    def __str__(self):
        return str(self.pk)

    def get_absolute_url(self):
        return reverse('admin:results_explorer_calculation_change', kwargs={'pk': self.pk})

    class Meta:
        verbose_name = "Result"
        verbose_name_plural = "Results"