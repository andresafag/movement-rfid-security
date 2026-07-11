locals {
  name_prefix = "${var.project_name}-${var.environment}"
}

# ── SNS Topic ─────────────────────────────────────────────────────────────────
# Receives notifications from the IoT Topic Rule and fans out to subscribers.
# Currently subscribed via email — see the aws_sns_topic_subscription below.

resource "aws_sns_topic" "sensor_alerts" {
  name              = "${local.name_prefix}-sensor-alerts"
  display_name      = "Sensor Access Alerts"
  kms_master_key_id = "alias/aws/sns"

  tags = {
    Name = "${local.name_prefix}-sensor-alerts"
  }
}

# ── Email Subscription ────────────────────────────────────────────────────────
# Subscribers receive every message published to the topic.
# NOTE: AWS will email the recipient a confirmation link that must be clicked
# once before messages start flowing.

resource "aws_sns_topic_subscription" "email" {
  for_each  = toset(var.subscriber_emails)
  topic_arn = aws_sns_topic.sensor_alerts.arn
  protocol  = "email"
  endpoint  = each.value
}
