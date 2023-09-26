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


from rest_framework import serializers

from workflow_explorer.models import Workflow


# Serializers define the API representation.

#
# PUBLIC ENDPOINT SERIALIZERS
#


class WorkflowSerializer(serializers.ModelSerializer):
    link = serializers.SerializerMethodField()

    class Meta:
        model = Workflow
        fields = ['name', 'user_id', 'workflow_model', 'link']

    def get_link(self, obj):
        link = "/api/workflows/" + str(obj.pk)
        return link


class WorkflowDetailSerializer(serializers.ModelSerializer):
    #
    # Top level (L1) workflow fields to expose
    #

    # Workflow name
    name = serializers.CharField()
    # Workflow ID (ATTN NOT THE DB PK!)
    # workflow_id = serializers.IntegerField(source='pk')
    workflow_id = serializers.CharField()
    # Purpose
    workflow_description = serializers.CharField()
    # Draft / Published (for client side filtering)
    workflow_status = serializers.SerializerMethodField()

    def get_workflow_status(self, obj):
        return obj.get_workflow_status_display()

    # Workflow Run Level (for creditNet and future complex models)
    run_level = serializers.IntegerField()
    # JSON Schema that validates the workflow
    api_version = serializers.CharField()
    # Workflow type (Not part of schema)
    workflow_type = serializers.SerializerMethodField()

    def get_workflow_type(self, obj):
        return obj.get_workflow_type_display()

    # Linked Model Definitions
    # Model Server (Name of Model Server Family used, eg. creditNet)
    # Model Server URL
    # Remote Model URL (for models served also as CGI independent of the Server)
    model_server = serializers.SerializerMethodField()

    def get_model_server(self, obj):
        return obj.workflow_model.model_server.model_server_name

    # Serialized but not in JSON Schema
    # The idea is that the model server where the workflow is posted
    # does not need to know its URL (duplication / source or errors)
    model_server_url = serializers.SerializerMethodField()

    def get_model_server_url(self, obj):
        return obj.workflow_model.model_server_url

    remote_model_url = serializers.SerializerMethodField()

    def get_remote_model_url(self, obj):
        remote_model_url = obj.workflow_model.remote_model_url
        return remote_model_url

    #
    # Nested L2 workflow fields to expose
    #

    # L2: Linked Model Configuration (expose all fields except some utility ones)
    # model_configuration = ModelConfigurationDetailSerializer()
    model_configuration = serializers.SerializerMethodField()

    def get_model_configuration(self, obj):
        if obj.model_configuration is not None:
            mc = ModelConfiguration.objects.get(pk=obj.model_configuration.pk)
            serializer = ModelConfigurationDetailSerializer(mc)
            model_configuration = serializer.data
        else:
            model_configuration = ''
        return model_configuration

    # L2: Linked Model Data Field (expose JSONField in its entirety)
    model_data = serializers.SerializerMethodField()

    def get_model_data(self, obj):
        if obj.model_data is not None:
            model_data = obj.model_data.model_data
        else:
            model_data = ''
        return model_data

    # L2: Input Set

    input_set = serializers.SerializerMethodField()

    def get_input_set(self, obj):
        if obj.portfolio is not None:
            # portfolio_link = ROOT_VIEW + "/api/portfolio_data/" + str(obj.portfolio.pk) + "/"
            portfolio_link = "portfolio_data/" + str(obj.portfolio.pk) + "/"
        elif obj.synthetic_portfolio is not None:
            # portfolio_link = ROOT_VIEW + "/api/synthetic_data/" + str(obj.synthetic_portfolio.pk) + "/"
            portfolio_link = "synthetic_data/" + str(obj.synthetic_portfolio.pk) + "/"
        else:
            portfolio_link = "None"

        if obj.liabilities is not None:
            liabilities_link = "entities/" + str(obj.liabilities.pk) + "/"
        else:
            liabilities_link = "None"

        if obj.transition_matrix is not None:
            # transition_matrix_link = ROOT_VIEW + "/api/transition_matrix/" + str(obj.transition_matrix.pk) + "/"
            transition_matrix_link = "transition_matrix/" + str(obj.transition_matrix.pk) + "/"
        else:
            transition_matrix_link = "None"

        if obj.correlation_matrix is not None:
            correlation_matrix_link = "correlation_matrix/" + str(obj.correlation_matrix.pk) + "/"
        else:
            correlation_matrix_link = "None"

        if obj.scenario is not None:
            scenario_link = "scenario_data/" + str(obj.scenario.pk) + "/"
        else:
            # scenario_data_link = obj.scenario_data_url
            scenario_link = "None"

        input_set = {'BaseAPIURL': "/api/",
                     'ExecDir': obj.root_dir,
                     'PortfolioURL': portfolio_link,
                     'LiabilitiesURL': liabilities_link,
                     'CorrelationURL': correlation_matrix_link,
                     'LoadingsURL': correlation_matrix_link,
                     'ScenarioDataURL': scenario_link,
                     'ThresholdDataURL': transition_matrix_link,
                     'TransitionDataURL': transition_matrix_link,
                     'Portfolio_data_mode': obj.portfolio_data_mode,
                     'Liability_data_mode': obj.liability_data_mode,
                     'Correlation_data_mode': obj.correlation_data_mode,
                     'Transition_data_mode': obj.transition_data_mode,
                     'Scenario_data_mode': obj.scenario_data_mode,
                     'Single_asset_flag': obj.single_asset_flag
                     }
        return input_set

    # L2: Output Set
    output_set = serializers.SerializerMethodField()

    def get_output_set(self, obj):
        output_set = {'Verbose_Level': obj.verbose_level,
                      'Reporting_mode': obj.reporting_mode,
                      'Results_url': obj.results_url,
                      'Results_list': obj.results_list,
                      'Scenario_output': obj.scenario_output}
        return output_set

    portfolio_data = serializers.SerializerMethodField()

    # L2: Linked Scenario Data
    # scenario_data = ScenarioDetailSerializer()

    class Meta:
        model = Workflow
        fields = ['input_set', 'output_set', 'model_configuration', 'model_data',
                  'remote_model_url', 'run_level', 'workflow_description', 'workflow_id',
                  'workflow_status', 'workflow_type', 'api_version', 'model_server',
                  'model_server_url', 'name', 'portfolio_data', 'scenario_data']
