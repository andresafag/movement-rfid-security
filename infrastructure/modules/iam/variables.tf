variable "project_name" {
  description = "Project name prefix"
  type        = string
}

variable "environment" {
  description = "Deployment environment"
  type        = string
}

variable "sns_topic_arn" {
  description = "ARN of the SNS topic the IoT rule engine will publish to"
  type        = string
}
