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

from django.contrib.auth.models import User
from django.db.models import JSONField
from django.db import models
from django.urls import reverse

OBJECTIVE_CHOICE = [(0, 'Portfolio Information'), (1, 'Concentration Risk'), (2, 'Origination'), (3, 'Risk Appetite'),
                    (4, 'Risk Capital'), (5, 'Other')]


class WorkflowJSON(models.Model):
    name = models.CharField(default="Default Name", max_length=200,
                            help_text="Assigned workflow name to help manage workflow collections")

    payload = JSONField(default=dict, null=True, blank=True,
                        help_text="Workflow JSON")

    def __str__(self):
        return self.name

    def get_absolute_url(self):
        return reverse('workflow_explorer:workflow_post', kwargs={'pk': self.pk})

    class Meta:
        verbose_name = "Workflow JSON"
        verbose_name_plural = "Workflow JSONs"


class Workflow(models.Model):
    WORKFLOW_STATUS_CHOICE = [(0, 'Draft'), (1, 'Published'), (2, 'Broken')]

    DEBUG = '0'
    BATCH = '1'
    INTERACTIVE = '2'
    # PARAMETRIC is obsolete (superseded by Playbook functionality)
    PARAMETRIC = '3'
    STUB = '4'
    TYPE_CHOICES = (
        (DEBUG, 'Debug'),
        (BATCH, 'Batch'),
        (INTERACTIVE, 'Interactive'),
        (PARAMETRIC, 'Parametric'),
        (STUB, 'Stub'),
    )

    PORTFOLIO_DATA_CHOICES = [(0, 'Load portfolio data from local JSON files'),
                              (1, 'Fetch portfolio data via REST API'),
                              (2, 'Create new portfolio from local JSON configuration'),
                              (3, 'Fetch portfolio configuration via REST API'),
                              (4, 'Attached Synthetic Configuration'),
                              (5, 'Attached Portfolio Data')]

    LIABILITY_DATA_CHOICES = [(0, 'No Liability data required'),
                              (1, 'Liability data from Local JSON File'),
                              (2, 'Liability data from API')]

    CORRELATION_DATA_CHOICES = [(0, 'No Correlation data required'),
                                (1, 'Correlation data from Local JSON File'),
                                (2, 'Correlation data from API')]

    TRANSITION_DATA_CHOICES = [(0, 'Read model data from local workflow, threshold data from api'),
                               (1, 'Read everything from local files')]

    SCENARIO_DATA_CHOICES = [(0, 'No Scenario data required'),
                             (1, 'Scenario data from Local JSON File'),
                             (2, 'Scenario data from API'),
                             (3, 'Attached Scenario Data')]

    REPORTING_CHOICE = ((n, str(n)) for n in range(4))
    VERBOSITY_CHOICE = ((n, str(n)) for n in range(4))
    RUN_LEVEL_CHOICE = ((2, 'Model Configuration'),
                        (3, 'Model Data'),
                        (4, 'Scenario Data'),
                        (5, 'Asset Data'),
                        (6, 'Liability Data'),
                        (7, 'Diagnostic Tests'),
                        (8, 'Model Calculation'),
                        (9, 'Reporting'))

    workflow_type = models.CharField(max_length=2, choices=TYPE_CHOICES, default=INTERACTIVE)

    objective = models.IntegerField(default=0, null=True, blank=True, choices=OBJECTIVE_CHOICE,
                                    help_text="Objective category of the workflow")

    creation_date = models.DateTimeField(auto_now_add=True)
    last_change_date = models.DateTimeField(auto_now=True)

    name = models.CharField(default="Default Name", max_length=200,
                            help_text="Assigned name to help manage workflow collections")

    workflow_id = models.CharField(default='CN_0000', max_length=200,
                                   help_text="Serial Number of the Workflow in the format MM_NNNN."
                                             "It is NOT the same as the database ID (pk) ")

    workflow_description = models.TextField(default="Default Description", null=True, blank=True,
                                            help_text="A description of the main purpose and "
                                                      "characteristics of the workflow")

    workflow_status = models.IntegerField(default=0, choices=WORKFLOW_STATUS_CHOICE,
                                          help_text='Draft/Published Status (Default=Draft)')

    # Run Level is only usefully for more complex models that have substantial internal structure
    run_level = models.IntegerField(default=7, null=True, blank=True, choices=RUN_LEVEL_CHOICE,
                                    help_text="Run level (for debug purposes only")

    api_version = models.CharField(default="0.4", max_length=50, null=True, blank=True,
                                   help_text="The API version to which the Workflow conforms")

    root_dir = models.CharField(default="", max_length=200, null=True, blank=True,
                                help_text="Root directory for distribution files")

    portfolio_data_mode = models.IntegerField(default=1, null=True, blank=True, choices=PORTFOLIO_DATA_CHOICES,
                                              help_text="Select the mode for portfolio data inputs")

    liability_data_mode = models.IntegerField(default=0, null=True, blank=True, choices=LIABILITY_DATA_CHOICES,
                                              help_text="Select the mode for liability data inputs")

    correlation_data_mode = models.IntegerField(default=1, null=True, blank=True, choices=CORRELATION_DATA_CHOICES,
                                                help_text="Select the mode for correlation data inputs")

    transition_data_mode = models.IntegerField(default=1, null=True, blank=True, choices=TRANSITION_DATA_CHOICES,
                                               help_text="Select the mode for transition data inputs")

    scenario_data_mode = models.IntegerField(default=1, null=True, blank=True, choices=SCENARIO_DATA_CHOICES,
                                             help_text="Select the mode for scenario data inputs")

    single_asset_flag = models.BooleanField(default=False, help_text="Set to TRUE for single asset portfolio")

    reporting_mode = models.IntegerField(default=0, null=True, blank=True, choices=REPORTING_CHOICE,
                                         help_text="The type of report to produce")

    results_url = models.CharField(default="", max_length=200, null=True, blank=True,
                                   help_text="Where to store the results")

    results_list = JSONField(default=dict, null=True, blank=True,
                             help_text="List of desired result items (when applicable")

    scenario_output = models.IntegerField(default=0, null=True, blank=True,
                                          help_text="What type of snapshot output to create")
    verbose_level = models.IntegerField(default=1, null=True, blank=True, choices=VERBOSITY_CHOICE,
                                        help_text="The verbosity level of logging")

    portfolio_data = JSONField(default=dict, null=True, blank=True,
                               help_text="Container for portfolio data. Not part of JSON Schema 0.4")

    scenario_data = JSONField(default=dict, null=True, blank=True,
                              help_text="Container for scenario data. Not part of JSON Schema 0.4")

    def __str__(self):
        return self.name

    def get_absolute_url(self):
        return reverse('workflow_explorer:workflow_view', kwargs={'pk': self.pk})

    class Meta:
        verbose_name = "Workflow"
        verbose_name_plural = "Workflows"
