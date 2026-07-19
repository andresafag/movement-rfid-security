# ── SNS topic for sensor notifications ─────────────────────────────
module "sns" {
  source = "./modules/sns"

  project_name      = var.project_name
  environment       = var.environment
  subscriber_emails = var.sns_subscriber_emails
}

# ── IAM role + policies for IoT Rules Engine ───────────────────────
module "iam" {
  source = "./modules/iam"

  project_name  = var.project_name
  environment   = var.environment
  sns_topic_arn = module.sns.topic_arn
}

# ── IoT Core (thing, cert, policy, Secrets Manager) ────────────────
module "iot_core" {
  source = "./modules/iot-core"

  project_name               = var.project_name
  environment                = var.environment
  aws_region                 = var.aws_region
  thing_name                 = var.thing_name
  topic_prefix               = var.topic_prefix
  aws_s3_bucket_firmware_arn = module.s3.aws_s3_bucket_firmware_arn

}

# ── IoT Topic Rule (MQTT → SNS notification) ───────────────────────
module "iot_rules" {
  source = "./modules/iot-rules"

  project_name      = var.project_name
  environment       = var.environment
  topic_prefix      = var.topic_prefix
  sns_topic_arn     = module.sns.topic_arn
  iot_rule_role_arn = module.iam.iot_rule_engine_role_arn
}

module "s3" {
  source = "./modules/s3"
}



