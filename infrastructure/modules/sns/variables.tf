variable "project_name" {
  description = "Project name prefix"
  type        = string
  default     = "sensoraccess"
}

variable "environment" {
  description = "Deployment environment"
  type        = string
}

variable "subscriber_emails" {
  description = "List of email addresses that will receive SNS notifications"
  type        = list(string)
  default     = ["andresfelipeacostagarcia34@gmail.com", "brigethedith01@gmail.com"]
}
